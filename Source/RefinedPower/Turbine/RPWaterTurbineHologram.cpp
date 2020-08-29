// ILikeBanas


#include "RPWaterTurbineHologram.h"
#include "FGWaterVolume.h"
#include "util/Logging.h"
#include "Containers/UnrealString.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/WeakObjectPtr.h"
#include "Components/BoxComponent.h"
#include "FGConstructDisqualifier.h"

ARPWaterTurbineHologram::ARPWaterTurbineHologram()
{
    mWaterTest = CreateDefaultSubobject<UBoxComponent>(TEXT("WaterTest"));
    mWaterTest->SetupAttachment(RootComponent);

    FVector boxSize = FVector(5, 5, 5);
    mWaterTest->SetBoxExtent(boxSize);

    mWaterTest->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bAllowTickOnDedicatedServer = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    PrimaryActorTick.SetTickFunctionEnable(true);

    mMaxPlacementFloorAngle = 90;
}

ARPWaterTurbineHologram::~ARPWaterTurbineHologram()
{
}

void ARPWaterTurbineHologram::BeginPlay()
{
    Super::BeginPlay();

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFGWaterVolume::StaticClass(), mCachedWaterVolArr);
}

bool ARPWaterTurbineHologram::CheckOverlapWaterVolume()
{
    if (mFoundWater && mWaterTest->IsOverlappingActor(mFoundWater))
    {
        return true;
    }

    mFoundWater = nullptr;

    for (int i = 0; i < mCachedWaterVolArr.Num(); i++)
    {
        AFGWaterVolume* water = (AFGWaterVolume*)mCachedWaterVolArr[i];

        // This WORKS!
        if (mWaterTest->IsOverlappingActor(water))
        {
            mFoundWater = water;
            return true;
        }
    }

    return false;
}

void ARPWaterTurbineHologram::CheckValidPlacement()
{
    Super::CheckValidPlacement();

    if (!mFoundWater)
    {
        //SML::Logging::info("[RefinedPower] - AddConstructDisqualifier");
        AddConstructDisqualifier(UFGCDInvalidPlacement::StaticClass());
        //AddConstructDisqualifier(UFGCDNeedsWaterVolume::StaticClass());
    }
}

void ARPWaterTurbineHologram::SetHologramLocationAndRotation(const FHitResult& hitResult)
{
    FVector TestLocation = FVector(hitResult.ImpactPoint.X, hitResult.ImpactPoint.Y, hitResult.ImpactPoint.Z);
    mWaterTest->SetWorldLocation(TestLocation);

    CheckOverlapWaterVolume();

    if (mFoundWater)
    {
        //SML::Logging::info("[RefinedPower] - Set Location to WATER?");
        FVector WaterOrigin;
        FVector WaterBounds;
        mFoundWater->GetActorBounds(false, WaterOrigin, WaterBounds);

        float WaterZ = WaterOrigin.Z;
        WaterZ += WaterBounds.Z;

        FVector location = FVector(hitResult.ImpactPoint.X, hitResult.ImpactPoint.Y, WaterZ);
        FRotator Rotation = FRotator(0, mScrollRotation, 0);
        this->SetActorLocationAndRotation(location, Rotation);
    }
    else
    {
        //SML::Logging::info("[RefinedPower] - Set Location to GROUND?");
        FVector location = FVector(hitResult.ImpactPoint.X, hitResult.ImpactPoint.Y, hitResult.ImpactPoint.Z);
        FRotator Rotation = FRotator(0, mScrollRotation, 0);

        this->SetActorLocationAndRotation(location, Rotation);
    }
}
