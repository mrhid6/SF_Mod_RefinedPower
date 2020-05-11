#include "RPTurbineBaseActor.h"
#include "util/Logging.h"
#include "Math/UnrealMathUtility.h"
#include "Containers/UnrealString.h"
#include "FGPowerConnectionComponent.h"
#include "FGPowerInfoComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UnrealNetwork.h"
#include "Engine.h"
#include "FGPlayerController.h"


void URPTurbineBaseRCO::SetTurbineEnabled_Implementation(ARPTurbineBaseActor* turbine, bool enabled){
	turbine->mTurbineEnabled = enabled;
	turbine->TurbineStateUpdated();

	turbine->calculateTurbinePowerProduction();
	turbine->ForceNetUpdate();
}

bool URPTurbineBaseRCO::SetTurbineEnabled_Validate(ARPTurbineBaseActor* turbine, bool enabled){
	return true;
}

void URPTurbineBaseRCO::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URPTurbineBaseRCO, bTest)
}

ARPTurbineBaseActor::ARPTurbineBaseActor() {

	FGPowerConnection = CreateDefaultSubobject<UFGPowerConnectionComponent>(TEXT("FGPowerConnection1"));
	FGPowerConnection->SetupAttachment(RootComponent);

	SetReplicates(true);
	bReplicates = true;

	mTurbineEnabled = true;
}

ARPTurbineBaseActor::~ARPTurbineBaseActor() {}

void ARPTurbineBaseActor::BeginPlay() {
	Super::BeginPlay();

	if (HasAuthority()) {
		FGPowerConnection->SetPowerInfo(GetPowerInfo());
	}

	calcNearbyWindTurbines();
	calculateTurbinePowerProduction();
	TurbineStateUpdated();
	
}

void ARPTurbineBaseActor::EndPlay(const EEndPlayReason::Type endPlayReason) {

	if (endPlayReason == EEndPlayReason::Destroyed) {
		calcNearbyWindTurbines();
	}
	Super::EndPlay(endPlayReason);
}

void ARPTurbineBaseActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPTurbineBaseActor, mTurbineEnabled);
	DOREPLIFETIME(ARPTurbineBaseActor, mWindTurbinesInArea);
}

bool ARPTurbineBaseActor::ShouldSave_Implementation() const {
	return true;
}

void ARPTurbineBaseActor::calculateTurbinePowerProduction() {

	if (mTurbineType == ETurbineType::RP_Wind) {
		if (mIsHeightBasedTurbine == true) {
			mTurbinePowerProduction = getTurbineHeightPowerProduction();
		}
		else {
			mTurbinePowerProduction = getTurbineBasePowerProduction();
		}

		if ((mWindTurbinesInArea + 1) >= mMaxWindTurbinesInArea) {
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
		TempFGPowerInfo->SetBaseProduction(powerOutput);
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

void ARPTurbineBaseActor::UpdateCachedNearbyWindTurbines() {
	const FVector ActorLocation = GetActorLocation();

	const TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes = TArray< TEnumAsByte< EObjectTypeQuery > >{ EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery2 };
	TArray< AActor*> ActorsToIgnore = TArray< AActor*>{ this };
	TArray< AActor*> OutActors;


	bool foundTurbines = UKismetSystemLibrary::SphereOverlapActors(this, ActorLocation, mAreaOfWindTurbines, ObjectTypes, ARPTurbineBaseActor::StaticClass(), ActorsToIgnore, OutActors);
	
	mCachedNearbyWindTurbines.Empty();

	if (foundTurbines) {

		for (const AActor* turbine : OutActors) {
			ARPTurbineBaseActor* RPTurbine = (ARPTurbineBaseActor*)turbine;

			if (RPTurbine->mTurbineType == ETurbineType::RP_Wind) {
				mCachedNearbyWindTurbines.Add(RPTurbine);
			}
		}
	}
}

int ARPTurbineBaseActor::getNearbyWindTurbinesCount() {
	return mWindTurbinesInArea;
}

void ARPTurbineBaseActor::calcNearbyWindTurbines() {

	if (mTurbineType == ETurbineType::RP_Water) {
		return;
	}

	updateNearbyWindTurbineCount();

	for (ARPTurbineBaseActor* turbine : mCachedNearbyWindTurbines) {
		turbine->updateNearbyWindTurbineCount();
		turbine->calculateTurbinePowerProduction();
		turbine->setTurbinePowerOutput();
	}
}

void ARPTurbineBaseActor::updateNearbyWindTurbineCount() {
	UpdateCachedNearbyWindTurbines();
	mWindTurbinesInArea = 0;
	mWindTurbinesInArea = FMath::Clamp(mCachedNearbyWindTurbines.Num(), 0, 9999);
}

bool ARPTurbineBaseActor::isTurbineEnabled() {
	return mTurbineEnabled;
}

void ARPTurbineBaseActor::setTurbineEnabled(bool turbineEnabled) {
	auto rco = Cast<URPTurbineBaseRCO>(Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(URPTurbineBaseRCO::StaticClass()));

	if (rco) {
		rco->SetTurbineEnabled(this, turbineEnabled);
	}
}
