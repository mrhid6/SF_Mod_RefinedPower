// ILikeBanas


#include "RPMPHeaterBuilding.h"

ARPMPHeaterBuilding::ARPMPHeaterBuilding() {

	SetReplicates(true);
	bReplicates = true;
	mFactoryTickFunction.SetTickFunctionEnable(true);
	mFactoryTickFunction.bCanEverTick = true;
}

void ARPMPHeaterBuilding::BeginPlay() {
	Super::BeginPlay();

	if (HasAuthority()) {
		CacheConnections();
	}

}
void ARPMPHeaterBuilding::Factory_Tick(float dt) {
	Super::Factory_Tick(dt);
	if (HasAuthority()) {
		CollectItems(dt);
	}

}
void ARPMPHeaterBuilding::Tick(float dt) {
	Super::Tick(dt);
}


void ARPMPHeaterBuilding::CacheConnections() {

	TArray<UActorComponent*> tempComps;

	FName tag = FName(TEXT("FuelConnection"));
	tempComps = GetComponentsByTag(UFGFactoryConnectionComponent::StaticClass(), tag);
	if (tempComps.Num() > 0) {
		InputFuelConveyor = Cast<UFGFactoryConnectionComponent>(tempComps[0]);
	}

	tag = FName(TEXT("FuelPipe"));
	tempComps = GetComponentsByTag(UFGPipeConnectionComponent::StaticClass(), tag);
	if (tempComps.Num() > 0) {
		InputFuelPipe = Cast<UFGPipeConnectionComponent>(tempComps[0]);
	}

	tag = FName(TEXT("Co2Pipe"));
	tempComps = GetComponentsByTag(UFGPipeConnectionComponent::StaticClass(), tag);
	if (tempComps.Num() > 0) {
		OutputCo2Pipe = Cast<UFGPipeConnectionComponent>(tempComps[0]);
	}

	if (OutputCo2Pipe == nullptr) {
		mProducesCo2 = false;
	}
}

void ARPMPHeaterBuilding::CollectItems(float dt)
{

	if (InputFuelConveyor) {
		for (auto AllowedItem : mAllowedFuelItems) {

			if (CanStoreItemInInventory(GetMPInventory(), mFuelInvIndex, AllowedItem, 1)) {
				FInventoryItem outItem;
				float offset = 0.0f;
				bool pulledItem = InputFuelConveyor->Factory_GrabOutput(outItem, offset, AllowedItem);
				if (pulledItem) {
					StoreItemInInventory(GetMPInventory(), mFuelInvIndex, AllowedItem, 1);
				}
			}
		}
	}

	if (InputFuelPipe) {
		for (auto AllowedItem : mAllowedFuelItems) {

			if (CanStoreItemInInventory(GetMPInventory(), mFuelInvIndex, AllowedItem, 100)) {
				FInventoryStack outStack;
				bool pulledItem = InputFuelPipe->Factory_PullPipeInput(dt, outStack, AllowedItem, 100);
				if (pulledItem) {
					StoreItemInInventory(GetMPInventory(), mFuelInvIndex, AllowedItem, 100);
				}
			}
		}
	}
}

int ARPMPHeaterBuilding::getFuelItemCount() {
	FInventoryStack out_stack;
	GetMPInventory()->GetStackFromIndex(mFuelInvIndex, out_stack);
	if (out_stack.HasItems()) {
		return out_stack.NumItems;
	}

	return 0;
}