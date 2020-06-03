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
	mWaterTest = CreateDefaultSubobject<UBoxComponent>(TEXT("WaterTest"));
	mWaterTest->SetupAttachment(RootComponent);

	FVector boxSize = FVector(200, 200, 200);
	mWaterTest->SetBoxExtent(boxSize);
	mWaterTest->SetHiddenInGameSML(false);

	mWaterTest->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	PrimaryActorTick.SetTickFunctionEnable(true);

	mMaxPlacementFloorAngle = 90;
}

ARPWaterTurbineHoloNew::~ARPWaterTurbineHoloNew() {}

void ARPWaterTurbineHoloNew::BeginPlay() {
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARPWaterTurbineNode::StaticClass(), mCachedWaterNodeArr);

	mSnapLocations.Add(FVector(325454.59f, -210184.08f, 7072.64f));
}

bool ARPWaterTurbineHoloNew::CheckSnapLocations(){

	mSnapToPoint = FVector(0);

	SML::Logging::info("[RefinedPower] - ", mCachedWaterNodeArr.Num());

	for (AActor* node : mCachedWaterNodeArr) {
		FVector location = node->GetActorLocation();

		bool foundSnap = IsPointInsideBox(location, mWaterTest);
		if(foundSnap){ 

			SML::Logging::info("[RefinedPower] - Found Snap Point!");
			mSnapToPoint = location;
			return true;
		}
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
	mWaterTest->SetWorldLocation(TestLocation);

	//CheckOverlapWaterVolume();
	CheckSnapLocations();

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

bool ARPWaterTurbineHoloNew::IsPointInsideBox(FVector point, UBoxComponent* boxComponent)
{
	float xmin = boxComponent->GetComponentLocation().X - boxComponent->GetScaledBoxExtent().X;
	float xmax = boxComponent->GetComponentLocation().X + boxComponent->GetScaledBoxExtent().X;
	float ymin = boxComponent->GetComponentLocation().Y - boxComponent->GetScaledBoxExtent().Y;
	float ymax = boxComponent->GetComponentLocation().Y + boxComponent->GetScaledBoxExtent().Y;
	float zmin = boxComponent->GetComponentLocation().Z - boxComponent->GetScaledBoxExtent().Z;
	float zmax = boxComponent->GetComponentLocation().Z + boxComponent->GetScaledBoxExtent().Z;

	if ((point.X <= xmax && point.X >= xmin) && (point.Y <= ymax && point.Y >= ymin) && (point.Z <= zmax && point.Z >= zmin))
	{
		return true;
	}
	return false;
}