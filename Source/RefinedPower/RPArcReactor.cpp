#include "RPArcReactor.h"
#include "FGFactoryConnectionComponent.h"
#include "FGPipeConnectionComponent.h"
#include "FGGameState.h"
#include "FGTimeSubsystem.h"
#include "UnrealNetwork.h"
#include "FGPowerInfoComponent.h"
#include "util/Logging.h"
#include "FGPowerConnectionComponent.h"

ARPArcReactor::ARPArcReactor() {
	//pwr
	UFGPowerInfoComponent* FGPowerInfo = CreateDefaultSubobject<UFGPowerInfoComponent>(TEXT("FGPowerInfo"));
	FGPowerConnection = CreateDefaultSubobject<UFGPowerConnectionComponent>(TEXT("FGPowerConnection"));
	FGPowerConnection->SetupAttachment(RootComponent);
	FGPowerConnection->SetPowerInfo(FGPowerInfo);

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
	InputConveyor = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("InputConveyor"));
	InputPipe = CreateDefaultSubobject<UFGPipeConnectionComponent>(TEXT("InputPipe"));

	InputConveyor->SetupAttachment(RootComponent);
	InputPipe->SetupAttachment(RootComponent);
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
}

void ARPArcReactor::Tick(float dt) {
	Super::Tick(dt);

	UpdateParticleVariables();
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
		TSubclassOf< class UFGItemDescriptor > fuel = GetCurrentFuelClass();
		int fuelAmnt = GetFuelInventory()->GetNumItems(fuel);
		if (fuelAmnt >= MinStartAmount) {
			ReactorState = EReactorState::RP_State_SpinUp;
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
		TSubclassOf< class UFGItemDescriptor > fuel = GetCurrentFuelClass();
		int fuelAmnt = GetFuelInventory()->GetNumItems(fuel);
		if (fuelAmnt <= MinStopAmount) {
			ReactorState = EReactorState::RP_State_SpinDown;
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
	ReactorSpinAmount++;
	ReactorSpinAmount = FMath::Clamp(ReactorSpinAmount, 0, 100);
	CalcSpinningState();
}

void ARPArcReactor::DecreaseSpinAmount() {
	ReactorSpinAmount--;
	ReactorSpinAmount = FMath::Clamp(ReactorSpinAmount, 0, 100);
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

/*#### End getters and setters*/

/*#######################################*/

//tick function - primary logic
void ARPArcReactor::Factory_Tick(float dt) {
	Super::Factory_Tick(dt);

	ToggleLight();

	CalcReactorState();

	if (mUpdateParticleVars && particlesEnabled) {
		UpdateParticleVariables();
	}
}