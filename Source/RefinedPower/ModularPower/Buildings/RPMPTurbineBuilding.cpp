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

	if (OutputSteamPipe) {
		OutputSteamPipe->SetInventory(mTurbineInventory);
		OutputSteamPipe->SetInventoryAccessIndex(mOutputInvIndex);
	}
}

void ARPMPTurbineBuilding::CollectSteam(float dt)
{	

	if (InputSteamPipe) {

		if (CanStoreItemInInventory(mTurbineInventory, mInputInvIndex, mHighSteamItemClass, mSteamPullAmount)) {
			FInventoryStack outStack;
			bool pulledItem = InputSteamPipe->Factory_PullPipeInput(dt, outStack, mHighSteamItemClass, mSteamPullAmount);

			if (pulledItem) {

				FInventoryStack TempOutStack;
				mTurbineInventory->GetStackFromIndex(mInputInvIndex, TempOutStack);
				if (TempOutStack.HasItems()) {
					int ItemCount = TempOutStack.NumItems;
				}

				StoreItemStackInInventory(mTurbineInventory, mInputInvIndex, outStack);
			}
		}
	}
}


void ARPMPTurbineBuilding::CalcTurbineState() {

	int PrevRPM = mCurrentTurbineRPM;

	if (CanStartSteamConsumption()) {
		ConvertSteamToRPM();
	}
	else {
		mSteamConsumptionRate = 0;
	}

	ReduceRPM();

	int NewRPM = mCurrentTurbineRPM;
	mRPMRate = NewRPM - PrevRPM;
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

	float ItemEnergyValue = UFGItemDescriptor::GetEnergyValue(mHighSteamItemClass);
	float ExtractAmount = 0;

	if (steamItemCount >= mSteamConsumption) {
		ExtractAmount = mSteamConsumption;
	}
	else {
		ExtractAmount = steamItemCount;
	}

	SML::Logging::info("[RefinedPower] - ExtractAmount:", ExtractAmount);

	mSteamConsumptionRate = ((ExtractAmount * 60)) / 1000;
	SML::Logging::info("[RefinedPower] - Consumption: ", mSteamConsumptionRate);

	mTurbineInventory->RemoveFromIndex(mInputInvIndex, ExtractAmount);

	int OutputAmount = ExtractAmount;

	ExtractAmount -= (ExtractAmount * mSteamDiscardPercent);
	OutputAmount += (ExtractAmount * mSteamDiscardPercent);

	OutputAmount /= 2;

	mSteamOutputRate = ((OutputAmount * 60)) / 1000;


	float EnergyValue = (ItemEnergyValue * ExtractAmount);
	EnergyValue = FMath::Clamp(EnergyValue, 0.0f, 9999999.0f);

	SML::Logging::info("[RefinedPower] - EnergyValue: ", EnergyValue);

	int RPMToAdd = FMath::FloorToInt((EnergyValue / 3) * mRPMMultiplier);

	mCurrentTurbineRPM += RPMToAdd;
	mCurrentTurbineRPM = FMath::Clamp(mCurrentTurbineRPM, 0, mRedMaxTurbineRPM);

	if (CanStoreItemInInventory(mTurbineInventory, mOutputInvIndex, mLowSteamItemClass, OutputAmount)) {
		StoreItemInInventory(mTurbineInventory, mOutputInvIndex, mLowSteamItemClass, OutputAmount);
	}
}

void ARPMPTurbineBuilding::ReduceRPM() {
	if (mCurrentTurbineRPM > 0) {
		mCurrentTurbineRPM -= mRPMDrag;

		mCurrentTurbineRPM = FMath::Clamp(mCurrentTurbineRPM, 0, mRedMaxTurbineRPM);
	}
}