#include "RPTurbineBaseActor.h"
#include "util/Logging.h"
#include "Math/UnrealMathUtility.h"
#include "Containers/UnrealString.h"
#include "FGPowerConnectionComponent.h"
#include "FGPowerInfoComponent.h"
#include "Kismet/KismetSystemLibrary.h"


ARPTurbineBaseActor::ARPTurbineBaseActor() {
	UFGPowerInfoComponent* FGPowerInfo = CreateDefaultSubobject<UFGPowerInfoComponent>(TEXT("FGPowerInfo1"));

	FGPowerConnection = CreateDefaultSubobject<UFGPowerConnectionComponent>(TEXT("FGPowerConnection1"));
	FGPowerConnection->SetupAttachment(RootComponent);
	FGPowerConnection->SetPowerInfo(FGPowerInfo);
}

void ARPTurbineBaseActor::BeginPlay() {
	calcInitalNearbyTurbines();
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

	if (FGPowerInfo != nullptr) {
		FGPowerInfo->SetBaseProduction(mTurbinePowerProduction);
	}
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

void ARPTurbineBaseActor::calcInitalNearbyTurbines() {
	const FVector ActorLocation = GetActorLocation();

	const TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes = TArray< TEnumAsByte< EObjectTypeQuery > >{ EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery2 };
	TArray< AActor*> ActorsToIgnore = TArray< AActor*>{ this };
	TArray< AActor*> OutActors;


	bool foundTurbines = UKismetSystemLibrary::SphereOverlapActors(this, ActorLocation, float(5200), ObjectTypes, ARPTurbineBaseActor::GetClass(), ActorsToIgnore, OutActors);

	if (foundTurbines) {

		for (const AActor* turbine: OutActors) {

		}

		mTurbinesInArea = OutActors.Num();
		SML::Logging::info("[RefinedPower] - Turbine Count: ", mTurbinesInArea);
	}
	else {
		SML::Logging::info("[RefinedPower] - Turbine Count: ", "None Found!");
		mTurbinesInArea = 0;
	}
}