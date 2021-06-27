// ILikeBanas

#include "Turbine/RPWaterTurbine.h"

void ARPWaterTurbine::BeginPlay()
{
    if (HasAuthority())
    {
        GetWaterTurbineNode();
    }

    Super::BeginPlay();
}

void ARPWaterTurbine::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    if (endPlayReason == EEndPlayReason::Destroyed)
    {
        if (mWaterTurbineNode)
        {
            mWaterTurbineNode->mHasTurbine = false;
            mWaterTurbineNode->OnHasTurbineChanged();
        }
    }
    Super::EndPlay(endPlayReason);
}

void ARPWaterTurbine::GetWaterTurbineNode()
{
    const FVector ActorLocation = GetActorLocation();
    const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = TArray<TEnumAsByte<EObjectTypeQuery>>{
        EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery2
    };
    TArray<AActor*> ActorsToIgnore = TArray<AActor*>{this};
    TArray<AActor*> OutActors;


    UKismetSystemLibrary::SphereOverlapActors(this, ActorLocation, 500, ObjectTypes, ARPWaterTurbineNode::StaticClass(),
                                              ActorsToIgnore, OutActors);
    if (OutActors.Num() > 0)
    {
        mWaterTurbineNode = Cast<ARPWaterTurbineNode>(OutActors[0]);
        mWaterTurbineNode->mHasTurbine = true;
        mWaterTurbineNode->OnHasTurbineChanged();
    }
}

void ARPWaterTurbine::calculateTurbinePowerProduction()
{
    if (mTurbineType == ETurbineType::RP_Water)
    {
        if (mWaterTurbineNode)
        {
            mTurbinePowerProduction = mWaterTurbineNode->GetTypePowerProduction();
        }
        else
        {
            mTurbinePowerProduction = 0;
        }
    }

    Super::calculateTurbinePowerProduction();
}
