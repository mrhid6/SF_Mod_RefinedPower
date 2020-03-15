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
	bReplicates = true;
	mFactoryTickFunction.bCanEverTick = true;

	/*############################################################*/
}

void ARPArcReactor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPArcReactor, ReactorState);
	DOREPLIFETIME(ARPArcReactor, ReactorSpinAmount);
}

void ARPArcReactor::BeginPlay() {
	Super::BeginPlay();

	FGPowerConnection->SetPowerInfo(GetPowerInfo());
	mUpdateParticleVars = true;
}

void ARPArcReactor::Tick(float dt) {
	Super::Tick(dt);

	if (mUpdateParticleVars) {
		UpdateParticleVariables();
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
void ARPArcReactor::ToggleLight() {
	//toggles light on and off depending on time of day
	AFGGameState* state = (AFGGameState*)UGameplayStatics::GetGameState(this);

	if (state == nullptr) {
		return;
	}

	AFGTimeOfDaySubsystem* timeSubsystem = state->GetTimeSubsystem();

	if (timeSubsystem == nullptr) {
		return;
	}

	if (timeSubsystem->IsNight() == true) {
		SpotLight->Activate();
	}
	else {
		SpotLight->Deactivate();
	}
}

void ARPArcReactor::CalcReactorState() {
	switch (ReactorState) {
	case EReactorState::RP_State_SpunDown:
	{
		if (getReactorCores() >= MinStartAmount) {
			SetReactorState(EReactorState::RP_State_SpinUp);
			CalcAudio();
		}
		break;
	}
	case EReactorState::RP_State_SpinUp:
	{
		IncreaseSpinAmount();
		if (ReactorSpinAmount >= 100) {
			SetReactorState(EReactorState::RP_State_Producing);
			RenderStateSpunUp();
			CalcAudio();
		}
		break;
	}
	case EReactorState::RP_State_Producing:
	{
		if (getReactorCores() <= MinStopAmount) {
			SetReactorState(EReactorState::RP_State_SpinDown);
			CalcAudio();
		}
		break;
	}
	case EReactorState::RP_State_SpinDown:
	{
		DecreaseSpinAmount();
		if (ReactorSpinAmount <= 0) {
			SetReactorState(EReactorState::RP_State_SpunDown);
			RenderStateSpunDown();
			CalcAudio();
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
	ReactorSpinAmount+= 0.02f;
	ReactorSpinAmount = FMath::Clamp(ReactorSpinAmount, 0.0f, 100.0f);
	CalcSpinningState();
}

void ARPArcReactor::DecreaseSpinAmount() {
	ReactorSpinAmount-= 0.02f;
	ReactorSpinAmount = FMath::Clamp(ReactorSpinAmount, 0.0f, 100.0f);
	CalcSpinningState();
}

void ARPArcReactor::CalcSpinningState() {
	float temp = ReactorSpinAmount * 0.01f;
	if (particlesEnabled) {
		SpinupRotation = FVector(0, 0, temp);
	}
	else {
		SpinupRotation = FVector(0, 0, 0);
	}
	SpinupOpacity = temp;

	mUpdateParticleVars = true;
}

void ARPArcReactor::RenderStateSpunDown() {
	SpinupRotation = FVector(0);
	SpinupOpacity = 0.0f;
	mUpdateParticleVars = true;
}

void ARPArcReactor::RenderStateSpunUp() {
	if (particlesEnabled) {
		SpinupRotation = FVector(0, 0, 1);
	}
	else {
		SpinupRotation = FVector(0, 0, 0);
	}
	SpinupOpacity = 1.0f;

	mUpdateParticleVars = true;
}

void ARPArcReactor::UpdateParticleVariables() {
	mUpdateParticleVars = false;

	PlasmaParticles->SetVectorParameter(FName("OrbitRate"), SpinupRotation);
	PlasmaParticles->SetFloatParameter(FName("PlasmaOpacity"), SpinupOpacity);

	if (ReactorSpinAmount <= 50) {
		PlasmaParticles->SetVectorParameter(FName("PlasmaColour"), FVector(1.0f, 0.0f, 0.0f));
	}
	else if (ReactorSpinAmount <= 75) {
		PlasmaParticles->SetVectorParameter(FName("PlasmaColour"), FVector(1.0f, 0.473217f, 0.0f));
	}
	else {
		PlasmaParticles->SetVectorParameter(FName("PlasmaColour"), FVector(0.22684f, 1.0f, 0.0f));
	}
}

void ARPArcReactor::CalcAudio() {
	if (mReactorSoundEnabled == true) {
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
	mReactorSoundEnabled = enabled;
	CalcAudio();
}

bool ARPArcReactor::isParticlesEnabled() {
	return particlesEnabled;
}

void ARPArcReactor::setParticlesEnabled(bool enabled) {
	particlesEnabled = enabled;
	if (!particlesEnabled) {
		SpinupRotation = FVector(0, 0, 0);
		SpinupOpacity = 0.0f;
	}
	else {
		CalcSpinningState();
	}

	mUpdateParticleVars = true;
}

int ARPArcReactor::getReactorSpinAmount() {
	return(ReactorSpinAmount);
}

int ARPArcReactor::getReactorCores() {
	FInventoryStack out_stack;
	bool gotFuel = GetFuelInventory()->GetStackFromIndex(mFuelInventoryIndex, out_stack);

	int fuelAmnt = 0;

	if (gotFuel) {
		fuelAmnt = out_stack.NumItems;
	}

	return fuelAmnt;
}

float ARPArcReactor::getReactorCoolantInternal() {
	FInventoryStack out_stack;
	bool gotFuel = GetFuelInventory()->GetStackFromIndex(mSupplementalInventoryIndex, out_stack);

	int coolantAmnt = 0;

	if (gotFuel) {
		coolantAmnt = out_stack.NumItems;
	}

	return coolantAmnt;
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

	ToggleLight();

	CalcReactorState();
}