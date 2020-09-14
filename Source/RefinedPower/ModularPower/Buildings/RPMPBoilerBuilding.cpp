// ILikeBanas


#include "RPMPBoilerBuilding.h"
#include "../RPMPPlacementComponent.h"

ARPMPBoilerBuilding::ARPMPBoilerBuilding()
{
    SetReplicates(true);
    bReplicates = true;
    mFactoryTickFunction.SetTickFunctionEnable(true);
    mFactoryTickFunction.bCanEverTick = true;
}

void ARPMPBoilerBuilding::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        CacheHeaterBuilding();
        CacheConnections();
    }
}

void ARPMPBoilerBuilding::Factory_Tick(float dt)
{
    Super::Factory_Tick(dt);
    if (HasAuthority())
    {
        CollectItems(dt);
        OutputSteam(dt);

        TransferToFluidBuffer();
        
        if (CanGenerateSteam())
        {
            GenerateSteam();
        }
    }
}

void ARPMPBoilerBuilding::Tick(float dt)
{
    Super::Tick(dt);
}

void ARPMPBoilerBuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void ARPMPBoilerBuilding::UpdateDependantBuildings()
{
    Super::UpdateDependantBuildings();
    CacheHeaterBuilding();
}


void ARPMPBoilerBuilding::CacheHeaterBuilding()
{
    if (mAttachedPlatform)
    {
        URPMPPlacementComponent* placementComp = mAttachedPlatform->GetPlacementComponent(
            EMPPlatformBuildingType::MP_Heater);


        if (placementComp->mAttachedBuilding)
        {
            if (placementComp->mAttachedBuilding->IsA(ARPMPHeaterBuilding::StaticClass()))
            {
                mAttachedHeater = Cast<ARPMPHeaterBuilding>(placementComp->mAttachedBuilding);
                //SML::Logging::info("[RefinedPower] - Got Heater!");
            }
        }
        else
        {
            mAttachedHeater = nullptr;
            //SML::Logging::info("[RefinedPower] - No Heater!");
        }
    }
    else
    {
        //SML::Logging::info("[RefinedPower] - CantFind Platform!");
    }
}

void ARPMPBoilerBuilding::CacheConnections()
{
    TArray<UActorComponent*> tempComps;

    FName tag = FName(TEXT("WaterInput"));
    tempComps = GetComponentsByTag(UFGPipeConnectionComponent::StaticClass(), tag);
    if (tempComps.Num() > 0)
    {
        InputWaterPipe = Cast<UFGPipeConnectionComponent>(tempComps[0]);
    }

    tag = FName(TEXT("SteamOutput"));
    tempComps = GetComponentsByTag(UFGPipeConnectionComponent::StaticClass(), tag);
    if (tempComps.Num() > 0)
    {
        OutputSteamPipe = Cast<UFGPipeConnectionComponent>(tempComps[0]);
    }

    if (OutputSteamPipe)
    {
        OutputSteamPipe->SetInventory(GetMPInventory());
        OutputSteamPipe->SetInventoryAccessIndex(mSteamInvIndex);
    }
}

float ARPMPBoilerBuilding::GetSteamProductionCurveValue()
{
    if (mAttachedHeater)
    {
        return mBoilerCurve->GetFloatValue(mAttachedHeater->getCurrentHeatValue());
    }

    return 0.0f;
}

void ARPMPBoilerBuilding::CollectItems(float dt)
{
    if (InputWaterPipe)
    {
        if (CanStoreItemInInventory(GetMPInventory(), mWaterInvIndex, mWaterItemClass, mWaterPullAmount))
        {
            FInventoryStack outStack;
            bool pulledItem = InputWaterPipe->Factory_PullPipeInput(dt, outStack, mWaterItemClass, mWaterPullAmount);

            if (pulledItem)
            {
                StoreItemStackInInventory(GetMPInventory(), mWaterInvIndex, outStack);
            }
        }
    }
}


void ARPMPBoilerBuilding::OutputSteam(float dt)
{
    FInventoryStack steamItemStack;
    GetMPInventory()->GetStackFromIndex(mSteamInvIndex, steamItemStack);
    int LowSteamItemCount = steamItemStack.NumItems;

    if (LowSteamItemCount >= 100)
    {
        steamItemStack.NumItems = 100;
    }

    if (OutputSteamPipe && OutputSteamPipe->IsConnected())
    {
        FInventoryStack outStack;
        int32 TakenAmount = OutputSteamPipe->Factory_PushPipeOutput(dt, steamItemStack);

        if (steamItemStack.HasItems() && TakenAmount > 0)
        {
            GetMPInventory()->RemoveFromIndex(mSteamInvIndex, TakenAmount);
        }
    }
}

bool ARPMPBoilerBuilding::CanGenerateSteam()
{
    if (mCurrentFluidBufferAmount > 0 && getSteamItemCount() < 50000)
    {
        return (GetSteamProductionCurveValue() > 0);
    }

    return false;
}

void ARPMPBoilerBuilding::GenerateSteam()
{
    // New Shiney
    float ExtractAmount = (float)FMath::Min(mCurrentFluidBufferAmount, mAmountFromBufferToTake);

    mCurrentFluidBufferAmount -= ExtractAmount;
    mCurrentFluidBufferAmount = FMath::Clamp(mCurrentFluidBufferAmount, 0.0f, mFluidBufferToLoad);
    float SteamAmount = (ExtractAmount * GetSteamProductionCurveValue()) * mSteamProductionMultiplier;
    
    mConsumptionTotal += ExtractAmount;
    mProductionTotal += SteamAmount;

    StoreItemInInventory(GetMPInventory(), mSteamInvIndex, mSteamItemClass, SteamAmount);
}

int ARPMPBoilerBuilding::getWaterItemCount()
{
    FInventoryStack out_stack;
    GetMPInventory()->GetStackFromIndex(mWaterInvIndex, out_stack);
    if (out_stack.HasItems())
    {
        return out_stack.NumItems;
    }

    return 0;
}

int ARPMPBoilerBuilding::getSteamItemCount()
{
    FInventoryStack out_stack;
    GetMPInventory()->GetStackFromIndex(mSteamInvIndex, out_stack);
    if (out_stack.HasItems())
    {
        return out_stack.NumItems;
    }

    return 0;
}


// Fluid Buffer

bool ARPMPBoilerBuilding::CanTransferToFluidBuffer()
{
    if (getWaterItemCount() > mFluidBufferToLoad)
    {
        return (mCurrentFluidBufferAmount == 0);
    }
    return false;
}

void ARPMPBoilerBuilding::TransferToFluidBuffer()
{
    if (CanTransferToFluidBuffer())
    {
        GetMPInventory()->RemoveFromIndex(mWaterInvIndex, mFluidBufferToLoad);
        mCurrentFluidBufferAmount = mFluidBufferToLoad;
    }
}
