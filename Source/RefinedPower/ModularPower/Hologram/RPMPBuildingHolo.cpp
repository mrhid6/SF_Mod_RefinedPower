// ILikeBanas


#include "RPMPBuildingHolo.h"
#include "util/Logging.h"
#include "Containers/UnrealString.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/WeakObjectPtr.h"
#include "FGConstructDisqualifier.h"

ARPMPBuildingHolo::ARPMPBuildingHolo() {

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	PrimaryActorTick.SetTickFunctionEnable(true);

	mMaxPlacementFloorAngle = 90;
}

ARPMPBuildingHolo::~ARPMPBuildingHolo() {}

void ARPMPBuildingHolo::BeginPlay() {
	Super::BeginPlay();
}

bool ARPMPBuildingHolo::CheckSnapLocations(FVector TestLocation){

	mSnapToPoint = FVector(0);
	const TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes = TArray< TEnumAsByte< EObjectTypeQuery > >{ EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery2 };
	TArray< AActor*> ActorsToIgnore = TArray< AActor*>{ this };
	TArray< AActor*> OutActors;


	UKismetSystemLibrary::SphereOverlapActors(this, TestLocation, 200, ObjectTypes, ARPMPPlatform::StaticClass(), ActorsToIgnore, OutActors);
	if (OutActors.Num() > 0) {
		mPlatform = Cast<ARPMPPlatform>(OutActors[0]);
	}
	else {
		mPlatform = nullptr;
	}

	if(mPlatform){
		TArray<UActorComponent*> placementComps = mPlatform->GetComponentsByClass(URPMPPlacementComponent::StaticClass());

		for (UActorComponent* comp : placementComps) {
			URPMPPlacementComponent* placementComp = Cast<URPMPPlacementComponent>(comp);

			if (placementComp->mBuildingType == mHologramType) {
				mSnapToPoint = placementComp->GetComponentLocation();
				return true;
			}
		}
	}

	return false;
}

void ARPMPBuildingHolo::CheckValidPlacement() {
	Super::CheckValidPlacement();

	if (mSnapToPoint == FVector(0)) {
		//SML::Logging::info("[RefinedPower] - AddConstructDisqualifier");
		//AddConstructDisqualifier(URPMPCDInvalidPlatformPlacement::StaticClass());
		AddConstructDisqualifier(UFGCDInvalidPlacement::StaticClass());
	}
}

void ARPMPBuildingHolo::SetHologramLocationAndRotation(const FHitResult& hitResult) {

	FVector TestLocation = FVector(hitResult.ImpactPoint.X, hitResult.ImpactPoint.Y, hitResult.ImpactPoint.Z);

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