// ILikeBanas


#include "RPWaterTurbineHologram.h"
#include "FGWaterVolume.h"
#include "util/Logging.h"
#include "Containers/UnrealString.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/WeakObjectPtr.h"
#include "Components/BoxComponent.h"
#include "FGConstructDisqualifier.h"

ARPWaterTurbineHologram::ARPWaterTurbineHologram() {
	mWaterTest = CreateDefaultSubobject<UBoxComponent>(TEXT("WaterTest"));
	mWaterTest->SetupAttachment(RootComponent);

	FVector boxSize = FVector(100, 100, 100);
	mWaterTest->SetBoxExtent(boxSize);

	mWaterTest->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	PrimaryActorTick.SetTickFunctionEnable(true);

	mMaxPlacementFloorAngle = 90;
}

void ARPWaterTurbineHologram::BeginPlay() {
	Super::BeginPlay();
}

void ARPWaterTurbineHologram::Tick(float dt) {
	Super::Tick(dt);

	SML::Logging::info("[RefinedPower] - TickTock!");
}

bool ARPWaterTurbineHologram::IsValidHitResult(const FHitResult& hitResult) const {
	Super::IsValidHitResult(hitResult);

	AFGWaterVolume* foundWater;
	return CheckOverlapWaterVolume(foundWater);
}

bool ARPWaterTurbineHologram::CheckOverlapWaterVolume(AFGWaterVolume* &foundWater) const {

	TArray<AActor*> foundWaterArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFGWaterVolume::StaticClass(), foundWaterArr);

	foundWater = nullptr;

	for (int i = 0; i < foundWaterArr.Num(); i++)
	{
		AFGWaterVolume* water = (AFGWaterVolume*)foundWaterArr[i];

		// This WORKS!
		if (mWaterTest->IsOverlappingActor(water)) {
			foundWater = water;
			return true;
		}
	}

	SML::Logging::info("[RefinedPower] - inValid Water Volume");
	return false;
}

void ARPWaterTurbineHologram::CheckValidPlacement() {
	Super::CheckValidPlacement();

	if (!CheckOverlapWaterVolume(mFoundWater)) {
		SML::Logging::info("[RefinedPower] - AddConstructDisqualifier");
		AddConstructDisqualifier(UFGCDNeedsWaterVolume::StaticClass());
	}
}

void ARPWaterTurbineHologram::SetHologramLocationAndRotation(const FHitResult& hitResult) {
	
	// Some weird shit going on here! probs IF statement?

	if (mFoundWater) {
		SML::Logging::info("[RefinedPower] - Set Location to WATER?");
		FVector WaterOrigin;
		FVector WaterBounds;
		mFoundWater->GetActorBounds(false, WaterOrigin, WaterBounds);

		float WaterZ = WaterOrigin.Z;
		WaterZ += WaterBounds.Z;

		FVector location = FVector(hitResult.ImpactPoint.X, hitResult.ImpactPoint.Y, WaterZ);
		FRotator Rotation = FRotator(0, mScrollRotation, 0);
		this->SetActorLocation(location, false);
		this->SetActorRotation(Rotation);
	}
}