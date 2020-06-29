// ILikeBanas


#include "RPMPTurbineBuilding.h"

ARPMPTurbineBuilding::ARPMPTurbineBuilding() {
	mTurbineInventory = CreateDefaultSubobject<UFGInventoryComponent>(TEXT("mTurbineInventory"));
	mTurbineInventory->SetDefaultSize(2);

	SetReplicates(true);
	bReplicates = true;
	mFactoryTickFunction.SetTickFunctionEnable(true);
	mFactoryTickFunction.bCanEverTick = true;
}

void ARPMPTurbineBuilding::BeginPlay() {
	Super::BeginPlay();

}
void ARPMPTurbineBuilding::Factory_Tick(float dt) {
	Super::Factory_Tick(dt);

}
void ARPMPTurbineBuilding::Tick(float dt) {
	Super::Tick(dt);
}