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
	ParticleData = CreateDefaultSubobject<UChildActorComponent>(TEXT("ParticleData"));
	PlasmaParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PlasmaParticles"));
	ParticleData->SetupAttachment(RootComponent);
	PlasmaParticles->SetupAttachment(RootComponent);

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

void ARPArcReactor::CalcReactorState(EReactorState RS) {
	switch (ReactorState) {
		case EReactorState::RP_State_SpinUp:
			IncreaseSpinAmount();
			break;
		case EReactorState::RP_State_Producing:
			//RenderStateSpunUp();
			ProduceMW();
			break;
		case EReactorState::RP_State_SpinDown:
			DecreaseSpinAmount();
			break;
		case EReactorState::RP_State_SpunDown:
			//RenderStateSpunDown();
			break;
	}
	RenderReactorState();
	//CalcAudio();
}

void ARPArcReactor::ReduceResourceAmounts() {
	//TODO
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
		//FIXME - Need particle data child
	}
}

void ARPArcReactor::RenderStateSpunDown() {
	SpinupRotation = FVector(0);
	SpinupOpacity = 0.0f;
}

void ARPArcReactor::ProduceMW() {
	this->FGPowerConnection->GetPowerInfo()->SetBaseProduction(getBaseReactorPowerProduction());
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
	//TODO
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

	/*CalcResourceState*/
	CalcResourceState();
	/*Delay 0.3s - Is this needed??*/

	/*CalcReactorState*/

	/*Delay 59.2s*/

	/*ReduceResourceAmounts*/
}