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
		CalcTurbineState();
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

	OutputSteamPipe->SetInventory(mTurbineInventory);
	OutputSteamPipe->SetInventoryAccessIndex(mOutputInvIndex);
}

void ARPMPTurbineBuilding::CollectSteam(float dt)
{	

	if (InputSteamPipe) {

		if (CanStoreItemInInventory(mTurbineInventory, mInputInvIndex, mHighSteamItemClass)) {
			FInventoryStack outStack;
			bool pulledItem = InputSteamPipe->Factory_PullPipeInput(dt, outStack, mHighSteamItemClass, 500);

			if (pulledItem) {

				FInventoryStack TempOutStack;
				mTurbineInventory->GetStackFromIndex(mInputInvIndex, TempOutStack);
				if (TempOutStack.HasItems()) {
					int ItemCount = TempOutStack.NumItems;
					SML::Logging::info(ItemCount);
				}

				StoreItemStackInInventory(mTurbineInventory, mInputInvIndex, outStack);
			}
		}
	}
}


void ARPMPTurbineBuilding::CalcTurbineState() {
	if (CanStartSteamConsumption()) {
		ConvertSteamToRPM();
	}

	ReduceRPM();
}

bool ARPMPTurbineBuilding::CanStartSteamConsumption() {
	FInventoryStack steamItemStack;
	mTurbineInventory->GetStackFromIndex(mInputInvIndex, steamItemStack);
	int steamItemCount = steamItemStack.NumItems;

	return (steamItemCount > 0);
}

void ARPMPTurbineBuilding::ConvertSteamToRPM() {

	

	FInventoryStack steamItemStack;
	mTurbineInventory->GetStackFromIndex(mInputInvIndex, steamItemStack);
	int steamItemCount = steamItemStack.NumItems;

	int ItemEnergyValue = UFGItemDescriptor::GetEnergyValue(mHighSteamItemClass);
	int ExtractAmount = 0;

	if (steamItemCount >= mSteamConsumption) {
		ExtractAmount = mSteamConsumption;
	}
	else {
		ExtractAmount = steamItemCount;
	}

	if (mSteamCounter == 0) {
		mSteamCounter = ExtractAmount;

		mTurbineInventory->RemoveFromIndex(mInputInvIndex, ExtractAmount);

		int OutputAmount = ExtractAmount;

		ExtractAmount -= (int)(ExtractAmount * mSteamDiscardPercent);
		OutputAmount += (int)(ExtractAmount * mSteamDiscardPercent);

		OutputAmount /= 2;


		float EnergyValue = (ItemEnergyValue * ExtractAmount);
		EnergyValue = FMath::Clamp(EnergyValue, 0.0f, 9999999.0f);

		SML::Logging::info("[RefinedPower] - ", EnergyValue);

		mCurrentTurbineRPM += (int)EnergyValue / 3;
		mCurrentTurbineRPM = FMath::Clamp(mCurrentTurbineRPM, 0, mRedMaxTurbineRPM);

		if (CanStoreItemInInventory(mTurbineInventory, mOutputInvIndex, mLowSteamItemClass)) {
			StoreItemInInventory(mTurbineInventory, mOutputInvIndex, mLowSteamItemClass, OutputAmount);
		}
	}else{
		mSteamCounter--;
		mSteamCounter = FMath::Clamp(mSteamCounter, 0, 9999999);
	}
}

void ARPMPTurbineBuilding::ReduceRPM() {
	if (mCurrentTurbineRPM > 0) {
		mCurrentTurbineRPM -= mRPMDrag;

		mCurrentTurbineRPM = FMath::Clamp(mCurrentTurbineRPM, 0, mRedMaxTurbineRPM);
	}
}