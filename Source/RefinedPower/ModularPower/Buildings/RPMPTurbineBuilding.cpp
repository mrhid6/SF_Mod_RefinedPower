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

	if (HasAuthority()) {
		CacheConnections();
	}

}
void ARPMPTurbineBuilding::Factory_Tick(float dt) {
	Super::Factory_Tick(dt);

	if (HasAuthority()) {
		CollectSteam(dt);
	}

}
void ARPMPTurbineBuilding::Tick(float dt) {
	Super::Tick(dt);
}


void ARPMPTurbineBuilding::CacheConnections() {

	TArray<UActorComponent*> tempComps;

	FName tag = FName(TEXT("SteamInput"));
	tempComps = GetComponentsByTag(UFGPipeConnectionComponent::StaticClass(), tag);
	if (tempComps.Num() > 0) {
		InputSteamPipe = Cast<UFGPipeConnectionComponent>(tempComps[0]);
	}

	tag = FName(TEXT("SteamOutput"));
	tempComps = GetComponentsByTag(UFGPipeConnectionComponent::StaticClass(), tag);
	if (tempComps.Num() > 0) {
		OutputSteamPipe = Cast<UFGPipeConnectionComponent>(tempComps[0]);
	}
}

void ARPMPTurbineBuilding::CollectSteam(float dt)
{	

	if (InputSteamPipe) {
		FInventoryStack outStack;
		bool pulledItem = InputSteamPipe->Factory_PullPipeInput(dt, outStack, mSteamItemClass);

		if (pulledItem) {
			StoreItemStackInInventory(mTurbineInventory, mInputInvIndex, outStack);
		}
	}
}
