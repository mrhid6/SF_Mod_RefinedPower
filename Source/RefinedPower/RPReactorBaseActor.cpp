#include "RPReactorBaseActor.h"
#include "util/Logging.h"
#include "Math/UnrealMathUtility.h"
#include "Containers/UnrealString.h"
#include "FGPowerConnectionComponent.h"
#include "FGPowerInfoComponent.h"
#include "FGFactoryConnectionComponent.h"
#include "FGPipeConnectionComponent.h"

ARPReactorBaseActor::ARPReactorBaseActor() {
	UFGPowerInfoComponent* FGPowerInfo = CreateDefaultSubobject<UFGPowerInfoComponent>(TEXT("FGPowerInfo1"));
	FGPowerConnection = CreateDefaultSubobject<UFGPowerConnectionComponent>(TEXT("FGPowerConnection1"));
	FGPowerConnection->SetupAttachment(RootComponent);
	FGPowerConnection->SetPowerInfo(FGPowerInfo);

	mReactorSoundEnabled = true;
}

void ARPReactorBaseActor::startReactorPowerProduction() {
	UFGPowerInfoComponent* FGPowerInfo = FGPowerConnection->GetPowerInfo();
	if (FGPowerInfo != nullptr) {
		FGPowerInfo->SetBaseProduction(mPowerProduction);
	}
}

void ARPReactorBaseActor::setBaseReactorPowerProduction(float p) {
	mPowerProduction = p;
}

float ARPReactorBaseActor::getBaseReactorPowerProduction() {
	return mPowerProduction;
}

//General function for grabbing resource from an input connection
int ARPReactorBaseActor::collectInputResource(UFGFactoryConnectionComponent* inputConnection, TSubclassOf<UFGItemDescriptor> type, const int maxStorage, int& inputAmount) {
	//check that amount is not too large
	if (inputAmount > maxStorage) {
		return inputAmount;
	}
	//dont need these for this implementation - left empty
	FInventoryItem out_item = FInventoryItem();
	float out_OffsetBeyond = 0.0f;

	//grabbing the item from the input and placing in internal storage
	if (inputConnection->Factory_GrabOutput(out_item,out_OffsetBeyond,type)) {
		inputAmount = FMath::Clamp(inputAmount++, 0, maxStorage);
		return inputAmount;
	}
	else {
		return inputAmount;
	}
}

int ARPReactorBaseActor::collectInputFluidResource(float dt, UFGPipeConnectionComponent* inputConnection, TSubclassOf<UFGItemDescriptor> type, const int maxStorage, int& inputAmount) {
	//check that amount is not too large
	if (inputAmount > maxStorage) {
		return inputAmount;
	}
	//dont need these for this implementation - left empty
	FInventoryStack out_item = FInventoryStack();

	//grabbing the item from the input and placing in internal storage
	if (inputConnection->Factory_PullPipeInput(dt, out_item, type)) {
		inputAmount = FMath::Clamp(inputAmount++, 0, maxStorage);
		return inputAmount;
	}
	else {
		return inputAmount;
	}
}