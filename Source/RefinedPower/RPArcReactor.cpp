#include "RPArcReactor.h"
#include "FGFactoryConnectionComponent.h"
#include "FGGameState.h"
#include "FGTimeSubsystem.h"
#include "UnrealNetwork.h"
#include "FGPowerInfoComponent.h"

ARPArcReactor::ARPArcReactor() : ARPReactorBaseActor() {
	//pwr initialized with parent's constructor

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
	InputConveyor1 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("InputConveyor1"));
	InputConveyor2 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("InputConveyor2"));
	InputPipe1 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("InputPipe1"));
	InputConveyor1->SetupAttachment(RootComponent);
	InputConveyor2->SetupAttachment(RootComponent);
	InputPipe1->SetupAttachment(RootComponent);
	/*############################################################*/

	/*############### Settup default values for UPROPERTIES ###############*/
	SpinupRotation = FVector(0);
	SpinupOpacity = 0.0f;
	ReactorState = EReactorState::RP_State_SpunDown;
	ReactorSpinAmount = 0;
	ReactorPrevState = EReactorState::RP_State_SpunDown;
	InputConveyor1Amount = 0;
	InputConveyor2Amount = 0;
	InputPipe1Amount = 0;
	ARPReactorBaseActor::setBaseReactorPowerProduction(5000.0f);
	MaxResourceAmount = 2000;
	MinStartAmount = 1500;
	MinStopAmount = 1000;
	PowerProducedThisCycle = 0.0f;
	PowerValuePerCycle = 30000.0f;
	particlesEnabled = false;
	bReplicates = true;
	/*############################################################*/
}

void ARPArcReactor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPArcReactor, SpinupRotation);
	DOREPLIFETIME(ARPArcReactor, SpinupOpacity);
	DOREPLIFETIME(ARPArcReactor, ReactorState);
	DOREPLIFETIME(ARPArcReactor, ReactorSpinAmount);
	DOREPLIFETIME(ARPArcReactor, ReactorPrevState);
	DOREPLIFETIME(ARPArcReactor, InputConveyor1Amount);
	DOREPLIFETIME(ARPArcReactor, InputConveyor2Amount);
	DOREPLIFETIME(ARPArcReactor, InputPipe1Amount);
}

void ARPArcReactor::BeginPlay() {
	//left empty
}

/*########## Main Functions ##########*/
void ARPArcReactor::ToggleLight() {
	//toggles light on and off depending on time of day
	AFGGameState* state = (AFGGameState*)UGameplayStatics::GetGameState;
	if (state->GetTimeSubsystem()->IsNight()) {
		SpotLight->Activate();
	}
	else {
		SpotLight->Deactivate();
	}
}

void ARPArcReactor::SetReactorState(EReactorState state) {
	ReactorPrevState = ReactorState;
	ReactorState = state;
}

void ARPArcReactor::CalcResourceState() {
	if (InputConveyor1Amount >= MinStartAmount &&
		InputConveyor2Amount >= MinStartAmount &&
		InputPipe1Amount >= MinStartAmount &&
		ReactorState == EReactorState::RP_State_SpunDown &&
		ReactorSpinAmount == 0) {

		SetReactorState(EReactorState::RP_State_SpinUp);
	}
	else if ((InputConveyor1Amount < MinStopAmount || InputConveyor2Amount < MinStopAmount || InputPipe1Amount < MinStopAmount) && ReactorState == EReactorState::RP_State_Producing) {
			
		SetReactorState(EReactorState::RP_State_SpinDown);
	}
}

void ARPArcReactor::CalcReactorState() {
	switch (ReactorState) {
		case EReactorState::RP_State_SpinUp:
			IncreaseSpinAmount();
			break;
		case EReactorState::RP_State_Producing:
			RenderStateSpunUp();
			ProduceMW();
			break;
		case EReactorState::RP_State_SpinDown:
			DecreaseSpinAmount();
			break;
		case EReactorState::RP_State_SpunDown:
			RenderStateSpunDown();
			break;
	}
	RenderReactorState();
	CalcAudio();
}

void ARPArcReactor::ReduceResourceAmounts() {
	if (ReactorState != EReactorState::RP_State_Producing) {
		return;
	}
	else {
		FMath::Clamp(InputConveyor2Amount - 1, 0, MaxResourceAmount);
		FMath::Clamp(InputConveyor1Amount - 1, 0, MaxResourceAmount);
		FMath::Clamp(InputPipe1Amount - 1, 0, MaxResourceAmount);
	}
}

void ARPArcReactor::UpdatePowerProducedThisCycle(float dT) {
	float tempProduced = (this->FGPowerConnection->GetPowerInfo()->GetRegulatedDynamicProduction()) * dT;
	PowerProducedThisCycle += tempProduced;
}
/*####################*/

/*########## Utility Functions ##########*/
void ARPArcReactor::IncreaseSpinAmount() {
	FMath::Clamp(ReactorSpinAmount++, 0, 100);
	CalcSpinningState();
}

void ARPArcReactor::DecreaseSpinAmount() {
	FMath::Clamp(ReactorSpinAmount--, 0, 100);
	CalcSpinningState();
}

void ARPArcReactor::CalcSpinningState() {
	if (ReactorSpinAmount <= 0) {
		SetReactorState(EReactorState::RP_State_SpunDown);
	}
	else if (ReactorSpinAmount >= 100) {
		SetReactorState(EReactorState::RP_State_Producing);
	}
	else {
		float temp = ReactorSpinAmount * 0.01f;
		if (particlesEnabled) {
			SpinupRotation = FVector(0, 0, temp);
		}
		else {
			SpinupRotation = FVector(0, 0, 0);
		}
		SpinupOpacity = temp;
	}
}

void ARPArcReactor::RenderStateSpunDown() {
	SpinupRotation = FVector(0);
	SpinupOpacity = 0.0f;
}

void ARPArcReactor::RenderStateSpunUp() {
	if (particlesEnabled) {
		SpinupRotation = FVector(0, 0, 1);
	}
	else {
		SpinupRotation = FVector(0, 0, 0);
	}
	SpinupOpacity = 1.0f;
}

void ARPArcReactor::ProduceMW() {
	ARPReactorBaseActor::startReactorPowerProduction();
}

void ARPArcReactor::RenderReactorState() {
	PlasmaParticles->SetVectorParameter(FName("OrbitRate"), SpinupRotation);
	PlasmaParticles->SetFloatParameter(FName("PlasmaOpacity"), SpinupOpacity);
	SetReactorPlasmaColor();
}

void ARPArcReactor::SetReactorPlasmaColor() {
	if (ReactorSpinAmount <= 50) {
		PlasmaParticles->SetVectorParameter(FName("PlasmaColor"), FVector(1, 0, 0));
	}
	else if (ReactorSpinAmount <= 75) {
		PlasmaParticles->SetVectorParameter(FName("PlasmaColor"), FVector(1, 0.473217, 0));
	}
	else {
		PlasmaParticles->SetVectorParameter(FName("PlasmaColor"), FVector(.22684, 1, 0));
	}
}

void ARPArcReactor::CalcAudio() {
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

int ARPArcReactor::getReactorSpinAmount() {
	return(ReactorSpinAmount);
}
/*#######################################*/

//tick function - primary logic
void ARPArcReactor::Factory_Tick(float dT) {
	ToggleLight();

	/*##### Collect Inputs #####*/
	/*CollectInputConveyor1*/
	InputConveyor1Amount = ARPReactorBaseActor::collectInputResource(InputConveyor1, Conveyor1InputClass, MaxResourceAmount, InputConveyor1Amount);

	/*CollectInputConveyor2*/
	InputConveyor2Amount = ARPReactorBaseActor::collectInputResource(InputConveyor2, Conveyor2InputClass, MaxResourceAmount, InputConveyor2Amount);

	/*CollectInputPipe1*/
	InputPipe1Amount = ARPReactorBaseActor::collectInputResource(InputPipe1, Pipe1InputClass, MaxResourceAmount, InputPipe1Amount);
	/*##########################*/

	CalcResourceState();
	CalcReactorState();
	if (ReactorState == EReactorState::RP_State_Producing) {
		UpdatePowerProducedThisCycle(dT);
		if (PowerProducedThisCycle >= PowerValuePerCycle) {
			ReduceResourceAmounts();
			PowerProducedThisCycle = 0.0f;
		}
	}
	
}