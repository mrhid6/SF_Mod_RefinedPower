#include "RPArcReactor.h"
#include "FGFactoryConnectionComponent.h"
#include "FGPipeConnectionComponent.h"
#include "FGGameState.h"
#include "FGTimeSubsystem.h"
#include "UnrealNetwork.h"
#include "FGPowerInfoComponent.h"
#include "util/Logging.h"
#include "FGPowerConnectionComponent.h"
#include "FGInventoryComponent.h"
#include "FGInventoryLibrary.h"
#include "FGPlayerController.h"


void URPArcReactorRCO::SetParticlesEnabled_Implementation(ARPArcReactor* reactor, bool enabled) {
	reactor->mParticlesEnabled = enabled;
	reactor->CalcSpinningState();
	reactor->UpdateParticleVariables();
	reactor->ForceNetUpdate();
}

bool URPArcReactorRCO::SetParticlesEnabled_Validate(ARPArcReactor* reactor, bool enabled) {
	return true;
}

void URPArcReactorRCO::SetSoundEnabled_Implementation(ARPArcReactor* reactor, bool enabled) {
	reactor->mReactorSoundEnabled = enabled;
	reactor->CalcAudio();
	reactor->ForceNetUpdate();
}

bool URPArcReactorRCO::SetSoundEnabled_Validate(ARPArcReactor* reactor, bool enabled) {
	return true;
}

void URPArcReactorRCO::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URPArcReactorRCO, bTest)
}

ARPArcReactor::ARPArcReactor() {
	//pwr
	FGPowerConnection = CreateDefaultSubobject<UFGPowerConnectionComponent>(TEXT("FGPowerConnection"));
	FGPowerConnection->SetupAttachment(RootComponent);

	//spotlight
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spotlight"));
	SpotLight->SetupAttachment(RootComponent);

	//particles
	PlasmaParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PlasmaParticles"));
	PlasmaParticles->SetupAttachment(RootComponent);

	//sound
	ArcReactorSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ArcReactorSound"));
	ArcReactorSound->SetupAttachment(RootComponent);

	/*############### Settup machine item inputs ###############*/
	InputConveyor = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("ConveyorInput1"));
	InputPipe = CreateDefaultSubobject<UFGPipeConnectionComponent>(TEXT("PipeInput1"));

	InputConveyor->SetupAttachment(RootComponent);
	InputPipe->SetupAttachment(RootComponent);

	mFuelInventory = CreateDefaultSubobject<UFGInventoryComponent>(TEXT("FuelInventory"));
	/*############################################################*/

	/*############### Settup tick values ###############*/
	SetReplicates(true);
	bReplicates = true;
	mFactoryTickFunction.bCanEverTick = true;

	/*############################################################*/
}

ARPArcReactor::~ARPArcReactor() {}

void ARPArcReactor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPArcReactor, ReactorState);
	DOREPLIFETIME(ARPArcReactor, mReactorSpinAmount);
	DOREPLIFETIME(ARPArcReactor, RPFuelInvIndex);
	DOREPLIFETIME(ARPArcReactor, RPCoolantInvIndex);

	DOREPLIFETIME(ARPArcReactor, mCachedReactorCoresAmount);
	DOREPLIFETIME(ARPArcReactor, mCachedCoolantAmount);

	DOREPLIFETIME(ARPArcReactor, mUpdateParticleVars);
	DOREPLIFETIME(ARPArcReactor, mUpdateAudio);

	DOREPLIFETIME(ARPArcReactor, mParticlesEnabled);
	DOREPLIFETIME(ARPArcReactor, mReactorSoundEnabled);

	DOREPLIFETIME(ARPArcReactor, mSpinupRotation);
	DOREPLIFETIME(ARPArcReactor, mSpinupOpacity);
}

void ARPArcReactor::BeginPlay() {
	Super::BeginPlay();

	if (HasAuthority()) {
		FGPowerConnection->SetPowerInfo(GetPowerInfo());
		RPFuelInvIndex = mFuelInventoryIndex;
		RPCoolantInvIndex = mSupplementalInventoryIndex;
	}

	mUpdateParticleVars = true;
	mUpdateAudio = true;
}

void ARPArcReactor::Tick(float dt) {
	Super::Tick(dt);

	if (mUpdateParticleVars) {
		UpdateParticleVariables();
	}

	if (mUpdateAudio) {
		CalcAudio();
	}
}

bool ARPArcReactor::CanStartPowerProduction_Implementation() const {
	Super::CanStartPowerProduction_Implementation();

	if (ReactorState == EReactorState::RP_State_Producing) {
		return true;
	}
	else {
		return false;
	}
}

/*########## Main Functions ##########*/

void ARPArcReactor::CalcReactorState() {
	switch (ReactorState) {
	case EReactorState::RP_State_SpunDown:
	{
		if (getReactorCores() >= MinStartAmount && getReactorCoolantInternal() >= 20000) {
			SetReactorState(EReactorState::RP_State_SpinUp);
			mUpdateAudio = true;
		}
		break;
	}
	case EReactorState::RP_State_SpinUp:
	{
		IncreaseSpinAmount();
		if (mReactorSpinAmount >= 100.0f) {
			SetReactorState(EReactorState::RP_State_Producing);
			RenderStateSpunUp();
			mUpdateAudio = true;
		}
		break;
	}
	case EReactorState::RP_State_Producing:
	{
		if (getReactorCores() <= MinStopAmount || getReactorCoolantInternal() < 10000) {
			SetReactorState(EReactorState::RP_State_SpinDown);
			mUpdateAudio = true;
		}
		break;
	}
	case EReactorState::RP_State_SpinDown:
	{
		DecreaseSpinAmount();
		if (mReactorSpinAmount <= 0.0f) {
			SetReactorState(EReactorState::RP_State_SpunDown);
			RenderStateSpunDown();
			mUpdateAudio = true;
		}
		break;
	}
	default:
		break;
	}
}

/*####################*/

/*########## Utility Functions ##########*/
void ARPArcReactor::IncreaseSpinAmount() {
	mReactorSpinAmount+= 0.02f;
	mReactorSpinAmount = FMath::Clamp(mReactorSpinAmount, 0.0f, 100.0f);
	CalcSpinningState();
}

void ARPArcReactor::DecreaseSpinAmount() {
	mReactorSpinAmount-= 0.02f;
	mReactorSpinAmount = FMath::Clamp(mReactorSpinAmount, 0.0f, 100.0f);
	CalcSpinningState();
}

void ARPArcReactor::CalcSpinningState() {
	float temp = mReactorSpinAmount * 0.01f;
	if (mParticlesEnabled) {
		mSpinupRotation = FVector(0, 0, temp);
		mSpinupOpacity = temp;
	}
	else {
		mSpinupRotation = FVector(0, 0, 0);
		mSpinupOpacity = 0.0f;
	}

	mUpdateParticleVars = true;
}

void ARPArcReactor::RenderStateSpunDown() {
	mSpinupRotation = FVector(0);
	mSpinupOpacity = 0.0f;
	mUpdateParticleVars = true;
}

void ARPArcReactor::RenderStateSpunUp() {
	if (mParticlesEnabled) {
		mSpinupRotation = FVector(0, 0, 1);
	}
	else {
		mSpinupRotation = FVector(0, 0, 0);
	}
	mSpinupOpacity = 1.0f;

	mUpdateParticleVars = true;
}

void ARPArcReactor::UpdateParticleVariables() {
	mUpdateParticleVars = false;

	PlasmaParticles->SetVectorParameter(FName("OrbitRate"), mSpinupRotation);
	PlasmaParticles->SetFloatParameter(FName("PlasmaOpacity"), mSpinupOpacity);

	if (mReactorSpinAmount <= 50) {
		PlasmaParticles->SetVectorParameter(FName("PlasmaColour"), FVector(1.0f, 0.0f, 0.0f));
	}
	else if (mReactorSpinAmount <= 75) {
		PlasmaParticles->SetVectorParameter(FName("PlasmaColour"), FVector(1.0f, 0.473217f, 0.0f));
	}
	else {
		PlasmaParticles->SetVectorParameter(FName("PlasmaColour"), FVector(0.22684f, 1.0f, 0.0f));
	}
}

void ARPArcReactor::CalcAudio() {
	mUpdateAudio = false;

	UFGGameUserSettings* UserSettings = UFGGameUserSettings::GetFGGameUserSettings();
	float Volume = UserSettings->GetAudioVolume("SFX_Bus_Volume") * 0.6;

	ArcReactorSound->SetVolumeMultiplier(Volume);

	if (mReactorSoundEnabled == true && ReactorState != EReactorState::RP_State_SpunDown) {
		if (!ArcReactorSound->IsPlaying()) {
			if (ReactorState == EReactorState::RP_State_SpinUp) {
				StartSpinupSound();
			}
			else if (ReactorState == EReactorState::RP_State_Producing) {
				StartProducingSound();
			}
			else if (ReactorState == EReactorState::RP_State_SpinDown) {
				StartShutdownSound();
			}
		}
	}
	else {
		if (ArcReactorSound->IsPlaying()) {
			ArcReactorSound->Stop();
		}
	}
}

void ARPArcReactor::CacheFuelAndCoolantAmount() {
	mCachedReactorCoresAmount = getReactorCores();
	mCachedCoolantAmount = getReactorCoolantInternal();
}

/*#### Getters & setters ####*/
void ARPArcReactor::SetReactorState(EReactorState state) {
	ReactorState = state;
}

EReactorState ARPArcReactor::GetReactorState() {
	return ReactorState;
}

bool ARPArcReactor::isSoundEnabled() {
	return mReactorSoundEnabled;
}

void ARPArcReactor::setSoundEnabled(bool enabled) {
	auto rco = Cast<URPArcReactorRCO>(Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(URPArcReactorRCO::StaticClass()));

	if (rco) {
		rco->SetSoundEnabled(this, enabled);
	}
}

bool ARPArcReactor::isParticlesEnabled() {
	return mParticlesEnabled;
}

void ARPArcReactor::setParticlesEnabled(bool enabled) {
	auto rco = Cast<URPArcReactorRCO>(Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(URPArcReactorRCO::StaticClass()));

	if (rco) {
		rco->SetParticlesEnabled(this, enabled);
	}
}

float ARPArcReactor::getReactorSpinAmount() {
	return mReactorSpinAmount;
}

int ARPArcReactor::getReactorCores() {
	if (HasAuthority()) {
		FInventoryStack out_stack;

		bool gotFuel = GetFuelInventory()->GetStackFromIndex(RPFuelInvIndex, out_stack);

		int fuelAmnt = 0;

		if (gotFuel) {
			fuelAmnt = out_stack.NumItems;
		}

		return fuelAmnt;
	}
	else {
		return mCachedReactorCoresAmount;
	}
}

float ARPArcReactor::getReactorCoolantInternal() {
	

	if (HasAuthority()) {
		FInventoryStack out_stack;

		bool gotCoolant = GetFuelInventory()->GetStackFromIndex(RPCoolantInvIndex, out_stack);

		int coolantAmnt = 0;

		if (gotCoolant) {
			coolantAmnt = out_stack.NumItems;
		}
		return coolantAmnt;
	}
	else {
		return mCachedCoolantAmount;
	}

	
}

float ARPArcReactor::getReactorCoolantInternalMax() {
	return float(50000);
}

float ARPArcReactor::getReactorCoolantBuffer() {
	return mCurrentSupplementalAmount;
}

float ARPArcReactor::getReactorCoolantBufferMax() {
	return mSupplementalLoadAmount;
}


/*#### End getters and setters*/

/*#######################################*/

//tick function - primary logic
void ARPArcReactor::Factory_Tick(float dt) {
	Super::Factory_Tick(dt);

	if (HasAuthority()) {
		CalcReactorState();
		CacheFuelAndCoolantAmount();

		ForceNetUpdate();
	}
}