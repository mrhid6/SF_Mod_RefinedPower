// ILikeBanas


#include "ModularPower/Buildings/RPMPGeneratorBuilding.h"
#include "FGPowerInfoComponent.h"

ARPMPGeneratorBuilding::ARPMPGeneratorBuilding()
{
    SetReplicates(true);
    bReplicates = true;
    mFactoryTickFunction.SetTickFunctionEnable(true);
    mFactoryTickFunction.bCanEverTick = true;
}

void ARPMPGeneratorBuilding::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        if (mAttachedPlatform)
        {
            mPowerInfo = mAttachedPlatform->GetPowerInfo();
        }

        CacheTurbineBuilding();
    }
}

void ARPMPGeneratorBuilding::Factory_Tick(float dt)
{
    Super::Factory_Tick(dt);
    if (HasAuthority())
    {
        ConvertRPMToPower();
    }
}

void ARPMPGeneratorBuilding::Tick(float dt)
{
    Super::Tick(dt);

    if (TriggerRPMChanged)
    {
        TriggerRPMChanged = false;
        Multicast_RPMChanged();
    }
}

void ARPMPGeneratorBuilding::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    if (endPlayReason == EEndPlayReason::Destroyed)
    {
        UFGPowerInfoComponent* powerInfo = GetPowerInfo();
        if (powerInfo != nullptr)
        {
            powerInfo->SetBaseProduction(0.0f);
        }
    }
    Super::EndPlay(endPlayReason);
}

void ARPMPGeneratorBuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARPMPGeneratorBuilding, mCurrentRPM);
    DOREPLIFETIME(ARPMPGeneratorBuilding, mCurrentPowerProduction);
}

void ARPMPGeneratorBuilding::UpdateDependantBuildings()
{
    Super::UpdateDependantBuildings();
    if (mAttachedPlatform)
    {
        mPowerInfo = mAttachedPlatform->GetPowerInfo();
    }
    CacheTurbineBuilding();
}

void ARPMPGeneratorBuilding::CacheTurbineBuilding()
{
    if (mAttachedPlatform)
    {
        URPMPPlacementComponent* placementComp = mAttachedPlatform->GetPlacementComponent(
            EMPPlatformBuildingType::MP_Turbine);


        if (placementComp->mAttachedBuilding)
        {
            if (placementComp->mAttachedBuilding->IsA(ARPMPTurbineBuilding::StaticClass()))
            {
                mAttachedTurbine = Cast<ARPMPTurbineBuilding>(placementComp->mAttachedBuilding);
                //SML::Logging::info("[RefinedPower] - Got Turbine!");
            }
        }
        else
        {
            mAttachedTurbine = nullptr;
            //SML::Logging::info("[RefinedPower] - No Turbine!");
        }
    }
    else
    {
        //SML::Logging::info("[RefinedPower] - CantFind Platform!");
    }
}


float ARPMPGeneratorBuilding::GetRPMPowerCurveValue()
{

    int prevRPM = mCurrentRPM;
    
    if (mAttachedTurbine != nullptr)
    {
        mCurrentRPM = mAttachedTurbine->mCurrentTurbineRPM;
        mCurrentPowerProduction = mGeneratorCurve->GetFloatValue(mCurrentRPM);

        mCurrentPowerProduction = FMath::Clamp(mCurrentPowerProduction, 0.0f, mMaxPowerOutput);
    }else
    {
        mCurrentRPM = 0;
        mCurrentPowerProduction = 0.0f;
    }

    if(prevRPM != mCurrentRPM)
    {
        TriggerRPMChanged = true;
    }

    return mCurrentPowerProduction;
}

void ARPMPGeneratorBuilding::ConvertRPMToPower()
{
    float prevPowerOutput = mCurrentPowerProduction;
    float powerOutput = GetRPMPowerCurveValue();
    if (prevPowerOutput != powerOutput)
    {
        UFGPowerInfoComponent* powerInfo = GetPowerInfo();
        if (powerInfo != nullptr)
        {
            powerInfo->SetBaseProduction(powerOutput);
        }
    }
}

void ARPMPGeneratorBuilding::Multicast_RPMChanged_Implementation()
{
    OnRep_RPMChanged();
}