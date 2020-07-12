// ILikeBanas


#include "RPMPCoolingBuilding.h"

ARPMPCoolingBuilding::ARPMPCoolingBuilding() {
	SetReplicates(true);
	bReplicates = true;
	mFactoryTickFunction.SetTickFunctionEnable(true);
	mFactoryTickFunction.bCanEverTick = true;
}

void ARPMPCoolingBuilding::BeginPlay() {
	Super::BeginPlay();

	if (HasAuthority()) {
		GetMPInventory()->SetDefaultSize(1);
		CacheConnections();
	}

}

void ARPMPCoolingBuilding::Factory_Tick(float dt) {
	Super::Factory_Tick(dt);
	if (HasAuthority()) {
		CollectItems(dt);

		if (CanEjectGas()) {
			isEjectingGas = true;
			mIsEmittingParticles = true;
			EjectGas();
		}
		else {
			isEjectingGas = false;
			mIsEmittingParticles = false;
			mGasConsumpionRate = 0.0f;
		}
	}

}
void ARPMPCoolingBuilding::Tick(float dt) {
	Super::Tick(dt);
}

void ARPMPCoolingBuilding::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPMPCoolingBuilding, mIsEmittingParticles)
	DOREPLIFETIME(ARPMPCoolingBuilding, mGasConsumpionRate)
}

void ARPMPCoolingBuilding::CacheConnections() {

	TArray<UActorComponent*> tempComps;

	FName tag = FName(TEXT("GasInputPipe"));
	tempComps = GetComponentsByTag(UFGPipeConnectionComponent::StaticClass(), tag);
	if (tempComps.Num() > 0) {
		InputPipe = Cast<UFGPipeConnectionComponent>(tempComps[0]);
	}
}

void ARPMPCoolingBuilding::CollectItems(float dt) {
	if (InputPipe) {

		if (CanStoreItemInInventory(GetMPInventory(), mInputInvIndex, mInputItemClass, mInputPullAmount)) {
			FInventoryStack outStack;
			bool pulledItem = InputPipe->Factory_PullPipeInput(dt, outStack, mInputItemClass, mInputPullAmount);

			if (pulledItem) {
				StoreItemStackInInventory(GetMPInventory(), mInputInvIndex, outStack);
			}
		}
	}
}

bool ARPMPCoolingBuilding::CanEjectGas() {
	if (getGasItemCount() > 0 && getGasItemCount() >= mMinEjectAmount) {
		return true;
	}
	else if (getGasItemCount() > 0 && isEjectingGas) {
		return true;
	}

	return false;
}


int ARPMPCoolingBuilding::getGasItemCount() {
	FInventoryStack out_stack;
	GetMPInventory()->GetStackFromIndex(mInputInvIndex, out_stack);
	if (out_stack.HasItems()) {
		return out_stack.NumItems;
	}

	return 0;
}


void ARPMPCoolingBuilding::EjectGas() {

	float ExtractAmount = 0;

	if (getGasItemCount() >= mGasUsageAmount) {
		ExtractAmount = mGasUsageAmount;
	}
	else {
		ExtractAmount = getGasItemCount();
	}

	mGasConsumpionRate = (ExtractAmount * 60.0f) / 1000.0f;
	GetMPInventory()->RemoveFromIndex(mInputInvIndex, ExtractAmount);
}