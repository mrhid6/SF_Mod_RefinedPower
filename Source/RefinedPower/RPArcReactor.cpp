#include "RPArcReactor.h"
#include "FGFactoryConnectionComponent.h"

ARPArcReactor::ARPArcReactor() : ARPReactorBaseActor() {
	//pwr initialized with parent's constructor
	InputConveyor1 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("InputConveyor1"));
	InputConveyor2 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("InputConveyor2"));
	InputPipe1 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("InputPipe1"));
	InputConveyor1->SetupAttachment(RootComponent);
	InputConveyor2->SetupAttachment(RootComponent);
	InputPipe1->SetupAttachment(RootComponent);
}

void ARPArcReactor::BeginPlay() {
	//TODO
}

void ARPArcReactor::CalcResourceState() {
	//TODO
}

void ARPArcReactor::CalcReactorState() {
	//TODO
}

void ARPArcReactor::ReduceResourceAmounts() {
	//TODO
}