// ILikeBanas


#include "RPMPGeneratorBuilding.h"
#include "../RPMPPlatform.h"


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
                SML::Logging::info("[RefinedPower] - Got Turbine!");
            }
        }
        else
        {
            mAttachedTurbine = nullptr;
            SML::Logging::info("[RefinedPower] - No Turbine!");
        }
    }
    else
    {
        SML::Logging::info("[RefinedPower] - CantFind Platform!");
    }
}


float ARPMPGeneratorBuilding::GetRPMPowerCurveValue()
{
    if (mAttachedTurbine)
    {
        mCurrentRPM = mAttachedTurbine->mCurrentTurbineRPM;
        mCurrentPowerProduction = mGeneratorCurve->GetFloatValue(mCurrentRPM);
        return mCurrentPowerProduction;
    }

    return 0.0f;
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
        ForceNetUpdate();
    }
}
