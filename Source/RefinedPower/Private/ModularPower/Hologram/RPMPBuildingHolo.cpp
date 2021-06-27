// ILikeBanas

#include "ModularPower/Hologram/RPMPBuildingHolo.h"
#include "Kismet/GameplayStatics.h"
#include "FGConstructDisqualifier.h"

ARPMPBuildingHolo::ARPMPBuildingHolo()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bAllowTickOnDedicatedServer = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    PrimaryActorTick.SetTickFunctionEnable(true);

    mMaxPlacementFloorAngle = 90;
}

ARPMPBuildingHolo::~ARPMPBuildingHolo()
{
}

void ARPMPBuildingHolo::BeginPlay()
{
    Super::BeginPlay();
}

bool ARPMPBuildingHolo::IsValidHitResult(const FHitResult& hitResult) const
{
    if (hitResult.IsValidBlockingHit() && hitResult.Actor != nullptr)
    {
        if (hitResult.Actor->IsA(ARPMPPlatform::StaticClass()) || hitResult.Actor->IsA(ARPMPBuilding::StaticClass()))
        {
            return true;
        }
    }

    return Super::IsValidHitResult(hitResult);
}

bool ARPMPBuildingHolo::CheckSnapLocations(FVector TestLocation)
{
    mSnapToPoint = FVector(0);
    mSnapPointRotation = FRotator(0);
    const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = TArray<TEnumAsByte<EObjectTypeQuery>>{
        EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery2
    };
    TArray<AActor*> ActorsToIgnore = TArray<AActor*>{this};
    TArray<AActor*> OutActors;


    UKismetSystemLibrary::SphereOverlapActors(this, TestLocation, 100, ObjectTypes, ARPMPPlatform::StaticClass(),
                                              ActorsToIgnore, OutActors);
    if (OutActors.Num() > 0)
    {
        mPlatform = Cast<ARPMPPlatform>(OutActors[0]);
    }
    else
    {
        mPlatform = nullptr;
    }


    if (mPlatform == nullptr)
    {
        UKismetSystemLibrary::SphereOverlapActors(this, TestLocation, 100, ObjectTypes, ARPMPBuilding::StaticClass(),
                                                  ActorsToIgnore, OutActors);
        if (OutActors.Num() > 0)
        {
            ARPMPBuilding* tempBuilding = Cast<ARPMPBuilding>(OutActors[0]);

            tempBuilding->GetAttachedPlatform(mPlatform);
        }
        else
        {
            mPlatform = nullptr;
        }
    }

    if (mPlatform)
    {
        TArray<UActorComponent*> placementComps = mPlatform->GetComponentsByClass(
            URPMPPlacementComponent::StaticClass());

        for (UActorComponent* comp : placementComps)
        {
            URPMPPlacementComponent* placementComp = Cast<URPMPPlacementComponent>(comp);

            if (placementComp->mBuildingType == mHologramType)
            {
                if (placementComp->IsOccupied())
                {
                    return false;
                }

                mSnapToPoint = placementComp->GetComponentLocation();
                mSnapPointRotation = placementComp->GetComponentRotation();
                return true;
            }
        }
    }

    return false;
}

void ARPMPBuildingHolo::CheckValidPlacement()
{
    Super::CheckValidPlacement();

    if (mSnapToPoint == FVector(0))
    {
        //SML::Logging::info("[RefinedPower] - AddConstructDisqualifier");
        //AddConstructDisqualifier(URPMPCDInvalidPlatformPlacement::StaticClass());
        AddConstructDisqualifier(UFGCDInvalidPlacement::StaticClass());
    }
}

void ARPMPBuildingHolo::SetHologramLocationAndRotation(const FHitResult& hitResult)
{
    FVector TestLocation = FVector(hitResult.ImpactPoint.X, hitResult.ImpactPoint.Y, hitResult.ImpactPoint.Z);

    //CheckOverlapWaterVolume();
    CheckSnapLocations(TestLocation);

    if (mSnapToPoint != FVector(0))
    {
        FVector location = mSnapToPoint;
        FRotator Rotation = mSnapPointRotation;
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
