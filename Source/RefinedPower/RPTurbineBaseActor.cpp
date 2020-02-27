#include "RPTurbineBaseActor.h"
#include "util/Logging.h"
#include "Math/UnrealMathUtility.h"
#include "Containers/UnrealString.h"
#include "FGPowerConnectionComponent.h"
#include "FGPowerInfoComponent.h"

ARPTurbineBaseActor::ARPTurbineBaseActor() {
	FGPowerConnection = CreateDefaultSubobject<UFGPowerConnectionComponent>(TEXT("FGPowerConnection1"));
	FGPowerConnection->SetupAttachment(RootComponent);
}

void ARPTurbineBaseActor::BeginPlay() {
	calculateTurbinePowerProduction();
	startTurbinePowerProduction();
}

void ARPTurbineBaseActor::calculateTurbinePowerProduction() {
	if (mIsHeightBasedTurbine == true) {
		mTurbinePowerProduction = getTurbineHeightPowerProduction();
	}
	else {
		mTurbinePowerProduction = getTurbinePowerProduction();
	}
}

void ARPTurbineBaseActor::startTurbinePowerProduction() {
	UFGPowerInfoComponent* FGPowerInfo = FGPowerConnection->GetPowerInfo();
	FGPowerInfo->SetBaseProduction(mTurbinePowerProduction);
}

float ARPTurbineBaseActor::getTurbinePowerProduction() {
	return mPowerProduction;
}

float ARPTurbineBaseActor::getTurbineHeightPowerProduction() {
	const FVector ActorLocation = GetActorLocation();

	float ActorHeight = ActorLocation.Z / 800;

	float tempPower = 0;

	if (ActorHeight < 7) {
		tempPower = mPowerProduction;
	}
	else {
		tempPower = ActorHeight;
	}

	tempPower -= mPowerProduction;
	tempPower *= float(2.1);
	tempPower += mPowerProduction;

	tempPower = FMath::Clamp(tempPower, float(10.0), float(30.0));
	tempPower = int(tempPower);

	SML::Logging::info("[RefinedPower] - Power Height: ", tempPower);

	return tempPower;
}