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
		OutputCo2(dt);

		if (CanStartItemBurn()) {
			BurnItem(dt);
		}
		else {
			subtractHeat(0.0166f);
		}
	}

}
void ARPMPHeaterBuilding::Tick(float dt) {
	Super::Tick(dt);
}

void ARPMPHeaterBuilding::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPMPHeaterBuilding, mCurrentHeatValue)
	DOREPLIFETIME(ARPMPHeaterBuilding, mCurrentEnergyValue)
	DOREPLIFETIME(ARPMPHeaterBuilding, mMaxEnergyValue)
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

		OutputCo2Pipe->SetInventory(GetMPInventory());
		OutputCo2Pipe->SetInventoryAccessIndex(mCo2InvIndex);
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

int ARPMPHeaterBuilding::getCo2ItemCount() {
	FInventoryStack out_stack;
	GetMPInventory()->GetStackFromIndex(mCo2InvIndex, out_stack);
	if (out_stack.HasItems()) {
		return out_stack.NumItems;
	}

	return 0;
}

bool ARPMPHeaterBuilding::CanStartItemBurn() {

	if ((getFuelItemCount() > 0 || mCurrentEnergyValue > 0) && getCo2ItemCount() < 50000) {
		return true;
	}

	return false;
}

void ARPMPHeaterBuilding::BurnItem(float dt) {

	if (mCurrentEnergyValue <= 0) { // No Items Being Burnt
		FInventoryStack FuelItemStack;
		GetMPInventory()->GetStackFromIndex(mFuelInvIndex, FuelItemStack);

		// Biomass - 4.5 Seconds;
		mCurrentEnergyValue = UFGItemDescriptor::GetEnergyValue(FuelItemStack.Item.ItemClass) * 1.5f / 60;
		mCurrentEnergyValue *= mEnergyValueMultiplier;

		mMaxEnergyValue = mCurrentEnergyValue;

		GetMPInventory()->RemoveFromIndex(mFuelInvIndex, 1);
	}
	else {
		mCurrentEnergyValue -= (1.0f * dt);

		mCurrentEnergyValue = FMath::Clamp(mCurrentEnergyValue, 0.0f, mMaxEnergyValue);
		addHeat(0.0166f);

		ProduceCo2();
	}
}

void ARPMPHeaterBuilding::ProduceCo2(){

	if (mProducesCo2) {
		if (CanStoreItemInInventory(GetMPInventory(), mCo2InvIndex, mCo2ItemClass, mCo2GenerationAmount)) {
			StoreItemInInventory(GetMPInventory(), mCo2InvIndex, mCo2ItemClass, mCo2GenerationAmount);
		}
	}
}

void ARPMPHeaterBuilding::OutputCo2(float dt) {
	FInventoryStack co2ItemStack;
	GetMPInventory()->GetStackFromIndex(mCo2InvIndex, co2ItemStack);
	int co2ItemCount = co2ItemStack.NumItems;

	if (co2ItemCount >= mCo2GenerationAmount) {
		co2ItemStack.NumItems = mCo2GenerationAmount;
	}

	if (OutputCo2Pipe && OutputCo2Pipe->IsConnected()) {
		FInventoryStack outStack;
		int32 TakenAmount = OutputCo2Pipe->Factory_PushPipeOutput(dt, co2ItemStack);

		if (co2ItemStack.HasItems() && TakenAmount > 0) {
			GetMPInventory()->RemoveFromIndex(mCo2InvIndex, TakenAmount);
		}
	}
}