#include "RPReactorBaseActor.h"
#include "util/Logging.h"
#include "Math/UnrealMathUtility.h"
#include "Containers/UnrealString.h"
#include "FGPowerConnectionComponent.h"
#include "FGPowerInfoComponent.h"
#include "FGFactoryConnectionComponent.h"

ARPReactorBaseActor::ARPReactorBaseActor() {
	UFGPowerInfoComponent* FGPowerInfo = CreateDefaultSubobject<UFGPowerInfoComponent>(TEXT("FGPowerInfo1"));
	FGPowerConnection = CreateDefaultSubobject<UFGPowerConnectionComponent>(TEXT("FGPowerConnection1"));
	FGPowerConnection->SetupAttachment(RootComponent);
	FGPowerConnection->SetPowerInfo(FGPowerInfo);
}

void ARPReactorBaseActor::startReactorPowerProduction() {
	UFGPowerInfoComponent* FGPowerInfo = FGPowerConnection->GetPowerInfo();
	if (FGPowerInfo != nullptr) {
		FGPowerInfo->SetBaseProduction(mReactorPowerProduction);
	}
}

float ARPReactorBaseActor::getReactorPowerProduction() {
	return mPowerProduction;
}

//General function for grabbing resource from an input connection
void ARPReactorBaseActor::collectInputResource(UFGFactoryConnectionComponent* inputConnection, TSubclassOf<UFGItemDescriptor> type, const int maxStorage, int& inputAmount) {
	//check that amount is not too large
	if (inputAmount > maxStorage) {
		return;
	}
	//dont need these for this implementation - left empty
	FInventoryItem out_item = FInventoryItem();
	float out_OffsetBeyond = 0.0f;

	//grabbing the item from the input and placing in internal storage
	if (inputConnection->Factory_GrabOutput(out_item,out_OffsetBeyond,type)) {
		inputAmount = FMath::Clamp(inputAmount++, 0, maxStorage);
	}
	else {
		return;
	}
}

/*children
//tick function - primary logic
void AFGBuildableFactory::Factory_Tick(float dT) {
	//check item inventory, call power function if item invetory has changed outside the bounds
}
*/