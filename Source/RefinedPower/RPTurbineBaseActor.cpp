#include "RPTurbineBaseActor.h"
#include "util/Logging.h"
#include "Math/UnrealMathUtility.h"
#include "Containers/UnrealString.h"
#include "FGPowerConnectionComponent.h"
#include "FGPowerInfoComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UnrealNetwork.h"
#include "Engine.h"


ARPTurbineBaseActor::ARPTurbineBaseActor() {

	FGPowerConnection = CreateDefaultSubobject<UFGPowerConnectionComponent>(TEXT("FGPowerConnection1"));
	FGPowerConnection->SetupAttachment(RootComponent);

	SetReplicates(true);
	bReplicates = true;
	mTurbineEnabled = true;
}

void ARPTurbineBaseActor::BeginPlay() {
	Super::BeginPlay();

	FGPowerConnection->SetPowerInfo(GetPowerInfo());


	calculateTurbinePowerProduction();
	updateTurbineParticleState();
}

void ARPTurbineBaseActor::EndPlay(const EEndPlayReason::Type endPlayReason) {

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

bool ARPTurbineBaseActor::ShouldSave_Implementation() const {
	return true;
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

		if (mWindTurbinesInArea >= mMaxWindTurbinesInArea) {
			mTurbinePowerProduction = 0;
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
	UFGPowerInfoComponent* TempFGPowerInfo = FGPowerConnection->GetPowerInfo();

	if (TempFGPowerInfo != nullptr) {
		const float powerOutput = getTurbineActualPowerProduction();
		SML::Logging::info("[RefinedPower] - Power Out: ", powerOutput);
		TempFGPowerInfo->SetBaseProduction(powerOutput);
		SML::Logging::info("[RefinedPower] - PowerInfo Out: ", TempFGPowerInfo->GetBaseProduction());
		//SML::Logging::info("[RefinedPower] - PowerInfo Out: ", mPowerInfo->GetBaseProduction());
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

float ARPTurbineBaseActor::getTurbineActualPowerProduction() {
	float powerOutput = 0.0f;
	if (mTurbineType == ETurbineType::RP_Wind) {
		powerOutput = mTurbinePowerProduction / float(getNearbyWindTurbinesCount() + 1);
	}
	else {
		powerOutput = mTurbinePowerProduction;
	}
	return powerOutput;
}

float ARPTurbineBaseActor::getMaxTurbinePowerProduction() {
	return mMaxTurbinePowerProduction;
}

TArray< ARPTurbineBaseActor*> ARPTurbineBaseActor::getNearbyWindTurbines() {
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

int ARPTurbineBaseActor::getNearbyWindTurbinesCount() {
	return mWindTurbinesInArea;
}

void ARPTurbineBaseActor::calcNearbyWindTurbines() {

	if (mTurbineType == ETurbineType::RP_Water) {
		return;
	}

	updateNearbyWindTurbineCount();
	TArray< ARPTurbineBaseActor*> TurbineArray = getNearbyWindTurbines();

	for (ARPTurbineBaseActor* turbine : TurbineArray) {
		turbine->updateNearbyWindTurbineCount();
		turbine->setTurbinePowerOutput();
	}
}

void ARPTurbineBaseActor::updateNearbyWindTurbineCount() {
	mWindTurbinesInArea = 0;
	TArray< ARPTurbineBaseActor*> TurbineArray = getNearbyWindTurbines();
	mWindTurbinesInArea = FMath::Clamp(TurbineArray.Num(), 0, 9999);
}

bool ARPTurbineBaseActor::isTurbineEnabled() {
	return mTurbineEnabled;
}

void ARPTurbineBaseActor::setTurbineEnabled(bool turbineEnabled) {
	mTurbineEnabled = turbineEnabled;
	calculateTurbinePowerProduction();
	updateTurbineParticleState();
}