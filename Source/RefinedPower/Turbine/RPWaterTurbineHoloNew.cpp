// ILikeBanas


#include "RPWaterTurbineHoloNew.h"
#include "FGWaterVolume.h"
#include "util/Logging.h"
#include "Containers/UnrealString.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/WeakObjectPtr.h"
#include "Components/BoxComponent.h"
#include "FGConstructDisqualifier.h"

ARPWaterTurbineHoloNew::ARPWaterTurbineHoloNew() {

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	PrimaryActorTick.SetTickFunctionEnable(true);

	mMaxPlacementFloorAngle = 90;
}

ARPWaterTurbineHoloNew::~ARPWaterTurbineHoloNew() {}

void ARPWaterTurbineHoloNew::BeginPlay() {
	Super::BeginPlay();
}

bool ARPWaterTurbineHoloNew::CheckSnapLocations(FVector TestLocation){

	mSnapToPoint = FVector(0);
	const TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes = TArray< TEnumAsByte< EObjectTypeQuery > >{ EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery2 };
	TArray< AActor*> ActorsToIgnore = TArray< AActor*>{ this };
	TArray< AActor*> OutActors;


	UKismetSystemLibrary::SphereOverlapActors(this, TestLocation, 500, ObjectTypes, ARPWaterTurbineNode::StaticClass(), ActorsToIgnore, OutActors);
	if (OutActors.Num() > 0) {
		mWaterTurbineNode = Cast<ARPWaterTurbineNode>(OutActors[0]);
	}
	else {
		mWaterTurbineNode = nullptr;
	}

	if(mWaterTurbineNode){
		mSnapToPoint = mWaterTurbineNode->GetActorLocation();
		return true;
	}

	return false;
}

void ARPWaterTurbineHoloNew::CheckValidPlacement() {
	Super::CheckValidPlacement();

	if (mSnapToPoint == FVector(0)) {
		//SML::Logging::info("[RefinedPower] - AddConstructDisqualifier");
		AddConstructDisqualifier(UFGCDInvalidPlacement::StaticClass());
		//AddConstructDisqualifier(UFGCDNeedsWaterVolume::StaticClass());
	}
}

void ARPWaterTurbineHoloNew::SetHologramLocationAndRotation(const FHitResult& hitResult) {

	FVector TestLocation = FVector(hitResult.ImpactPoint.X, hitResult.ImpactPoint.Y, hitResult.ImpactPoint.Z+100);

	//CheckOverlapWaterVolume();
	CheckSnapLocations(TestLocation);

	if (mSnapToPoint != FVector(0)) {

		FVector location = mSnapToPoint;
		FRotator Rotation = FRotator(0, mScrollRotation, 0);
		this->SetActorLocationAndRotation(location, Rotation);
	}
	else {
		//SML::Logging::info("[RefinedPower] - Set Location to GROUND?");
		FVector location = FVector(hitResult.ImpactPoint.X, hitResult.ImpactPoint.Y, hitResult.ImpactPoint.Z);
		FRotator Rotation = FRotator(0, mScrollRotation, 0);

		this->SetActorLocationAndRotation(location, Rotation);
	}
}