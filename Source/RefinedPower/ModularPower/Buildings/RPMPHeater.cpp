// ILikeBanas


#include "RPMPHeater.h"

ARPMPHeater::ARPMPHeater() {
	mHeaterInventory = CreateDefaultSubobject<UFGInventoryComponent>(TEXT("mHeaterInventory"));
	mHeaterInventory->SetDefaultSize(2);

	SetReplicates(true);
	bReplicates = true;
	mFactoryTickFunction.SetTickFunctionEnable(true);
	mFactoryTickFunction.bCanEverTick = true;
}

void ARPMPHeater::BeginPlay() {
	Super::BeginPlay();

	if (HasAuthority()) {
		CacheConnections();
	}

}
void ARPMPHeater::Factory_Tick(float dt) {
	Super::Factory_Tick(dt);
	if (HasAuthority()) {
		CollectItems();
	}

}
void ARPMPHeater::Tick(float dt) {
	Super::Tick(dt);
}


void ARPMPHeater::CacheConnections() {

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

	/*tag = FName(TEXT("WaterPipe"));
	tempComps = GetComponentsByTag(UFGPipeConnectionComponent::StaticClass(), tag);
	if (tempComps.Num() > 0) {
		InputWaterPipe = Cast<UFGPipeConnectionComponent>(tempComps[0]);
	}

	tag = FName(TEXT("SteamPipe"));
	tempComps = GetComponentsByTag(UFGPipeConnectionComponent::StaticClass(), tag);
	if (tempComps.Num() > 0) {
		OutputSteamPipe = Cast<UFGPipeConnectionComponent>(tempComps[0]);
	}*/

	tag = FName(TEXT("Co2Pipe"));
	tempComps = GetComponentsByTag(UFGPipeConnectionComponent::StaticClass(), tag);
	if (tempComps.Num() > 0) {
		OutputCo2Pipe = Cast<UFGPipeConnectionComponent>(tempComps[0]);
	}

	if (OutputCo2Pipe == nullptr) {
		mProducesCo2 = false;
	}
}

void ARPMPHeater::CollectItems()
{

	if (InputFuelConveyor) {
		for (auto AllowedItem : mAllowedFuelItems) {

			if (canAddFuelItem(AllowedItem)) {
				FInventoryItem outItem;
				float offset = 0.0f;
				bool pulledItem = InputFuelConveyor->Factory_GrabOutput(outItem, offset, AllowedItem);
				if (pulledItem) {
					FInventoryStack itemStack;
					itemStack.NumItems = 1;
					itemStack.Item = outItem;
					mHeaterInventory->AddStackToIndex(mFuelInvIndex, itemStack, true);
				}
			}
		}
	}

	// Copy above for InputFuelPipe

	// Copy above for InputWaterPipe - ***Boiler instead
}

bool ARPMPHeater::canAddFuelItem(TSubclassOf<UFGItemDescriptor> itemClass) {
	FInventoryStack out_stack;
	mHeaterInventory->GetStackFromIndex(mFuelInvIndex, out_stack);
	if (out_stack.HasItems()) {

		if (itemClass != out_stack.Item.ItemClass) {
			return false;
		}

		int stackSize = UFGItemDescriptor::GetStackSize(out_stack.Item.ItemClass);

		if (getFuelItemCount() >= stackSize) {
			return false;
		}
	}

	return true;
}

int ARPMPHeater::getFuelItemCount() {
	FInventoryStack out_stack;
	mHeaterInventory->GetStackFromIndex(mFuelInvIndex, out_stack);
	if (out_stack.HasItems()) {
		return out_stack.NumItems;
	}

	return 0;
}