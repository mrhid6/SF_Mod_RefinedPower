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

	mTurbineEnabled = true;
}

void ARPTurbineBaseActor::BeginPlay() {
	calculateTurbinePowerProduction();

	Super::BeginPlay();
}

void ARPTurbineBaseActor::EndPlay(const EEndPlayReason::Type endPlayReason) {

	SML::Logging::info("[RefinedPower] - EndPlay: 1");
	if (endPlayReason == EEndPlayReason::Destroyed) {
		SML::Logging::info("[RefinedPower] - EndPlay: 2");
		calcNearbyWindTurbines();
	}
	Super::EndPlay(endPlayReason);
}

void ARPTurbineBaseActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPTurbineBaseActor, mTurbineEnabled);
}

void ARPTurbineBaseActor::calculateTurbinePowerProduction() {

	if (mTurbineType == ETurbineType::RP_Wind) {
		calcNearbyWindTurbines();
		if (mIsHeightBasedTurbine == true) {
			mTurbinePowerProduction = getTurbineHeightPowerProduction();
		}
		else {
			mTurbinePowerProduction = getTurbineBasePowerProduction();
		}
	}
	else {
		mTurbinePowerProduction = getTurbineBasePowerProduction();
	}

	if (mTurbineEnabled == false) {
		mTurbinePowerProduction = 0;
	}

	setTurbinePowerOutput();
}

void ARPTurbineBaseActor::setTurbinePowerOutput() {
	UFGPowerInfoComponent* FGPowerInfo = FGPowerConnection->GetPowerInfo();

	if (FGPowerInfo != nullptr) {
		const float powerOutput = mTurbinePowerProduction / float(mWindTurbinesInArea + 1);
		SML::Logging::info("[RefinedPower] - Power Out: ", powerOutput);
		FGPowerInfo->SetBaseProduction(powerOutput);
		SML::Logging::info("[RefinedPower] - PowerInfo Out: ", FGPowerInfo->GetBaseProduction());
	}
}

float ARPTurbineBaseActor::getTurbineBasePowerProduction() {
	return mPowerProduction;
}

float ARPTurbineBaseActor::getTurbineHeightPowerProduction() {
	const FVector ActorLocation = GetActorLocation();

	float ActorHeight = ActorLocation.Z / 800;
	const float basePower = getTurbineBasePowerProduction();

	float tempPower = 0;

	if (ActorHeight < 7) {
		tempPower = basePower;
	}
	else {
		tempPower = ActorHeight;
	}

	tempPower -= basePower;
	tempPower *= float(2.1);
	tempPower += basePower;

	tempPower = FMath::Clamp(tempPower, float(10.0), float(30.0));
	tempPower = int(tempPower);

	SML::Logging::info("[RefinedPower] - Power Height: ", tempPower);

	return tempPower;
}

TArray< ARPTurbineBaseActor*> ARPTurbineBaseActor::getNearbyWindTurbineCount() {
	const FVector ActorLocation = GetActorLocation();

	const TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes = TArray< TEnumAsByte< EObjectTypeQuery > >{ EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery2 };
	TArray< AActor*> ActorsToIgnore = TArray< AActor*>{ this };
	TArray< AActor*> OutActors;

	TArray< ARPTurbineBaseActor*> TurbineArray;


	bool foundTurbines = UKismetSystemLibrary::SphereOverlapActors(this, ActorLocation, float(5200), ObjectTypes, ARPTurbineBaseActor::GetClass(), ActorsToIgnore, OutActors);
	

	if (foundTurbines) {

		for (const AActor* turbine : OutActors) {
			ARPTurbineBaseActor* RPTurbine = (ARPTurbineBaseActor*)turbine;

			if (RPTurbine->mTurbineType == ETurbineType::RP_Wind) {
				TurbineArray.Add(RPTurbine);
			}
		}
	}

	return TurbineArray;
}

void ARPTurbineBaseActor::calcNearbyWindTurbines() {

	if (mTurbineType == ETurbineType::RP_Water) {
		return;
	}

	updateNearbyWindTurbineCount();
	TArray< ARPTurbineBaseActor*> TurbineArray = getNearbyWindTurbineCount();

	for (ARPTurbineBaseActor* turbine : TurbineArray) {
		turbine->updateNearbyWindTurbineCount();
		turbine->setTurbinePowerOutput();
	}
}

void ARPTurbineBaseActor::updateNearbyWindTurbineCount() {
	mWindTurbinesInArea = 0;
	TArray< ARPTurbineBaseActor*> TurbineArray = getNearbyWindTurbineCount();
	mWindTurbinesInArea = FMath::Clamp(TurbineArray.Num(), 0, 9999);
}

void ARPTurbineBaseActor::updateTurbineEnabled(bool turbineEnabled) {
	mTurbineEnabled = turbineEnabled;
	calculateTurbinePowerProduction();
	updateTurbineParticleState();
}