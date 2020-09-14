// ILikeBanas


#include "RPMPSolutionHeaterBuilding.h"

ARPMPSolutionHeaterBuilding::ARPMPSolutionHeaterBuilding()
{
    SetReplicates(true);
    bReplicates = true;
    mFactoryTickFunction.SetTickFunctionEnable(true);
    mFactoryTickFunction.bCanEverTick = true;
}

void ARPMPSolutionHeaterBuilding::BeginPlay()
{
    Super::BeginPlay();
}

void ARPMPSolutionHeaterBuilding::Factory_Tick(float dt)
{
    Super::Factory_Tick(dt);

    if (HasAuthority())
    {
        TransferToFluidBuffer();
    }
}

bool ARPMPSolutionHeaterBuilding::CanStartItemBurn()
{
    if ((mCurrentFluidBufferAmount > 0 || mCurrentEnergyValue > 0) && (getCo2ItemCount() < 50000 && getCo2ItemCount() + mOutputGenerationAmount < 50000))
    {
        return (mCurrentHeatValue < mMaxHeatValue);
    }

    return false;
}

void ARPMPSolutionHeaterBuilding::BurnItem(float dt)
{
    if (mCurrentEnergyValue <= 0)
    {
        // No Items Being Burnt
        FInventoryStack FuelItemStack;
        GetMPInventory()->GetStackFromIndex(mInputInvIndex, FuelItemStack);

        // Figure out how much fuel we need to burn
        float ExtractAmount = (float)FMath::Min(mCurrentFluidBufferAmount, mAmountFromBufferToTake);
        
        // Biomass - 4.5 Seconds;
        float itemEnergyValue = UFGItemDescriptor::GetEnergyValue(FuelItemStack.Item.ItemClass);
        mCurrentEnergyValue = (itemEnergyValue / 60.0f);
        mCurrentEnergyValue *= ExtractAmount;
        mCurrentEnergyValue *= mEnergyValueMultiplier;

        //SML::Logging::info(mCurrentEnergyValue);

        mMaxEnergyValue = mCurrentEnergyValue;

        // Remove fluid from buffer

        mConsumptionTotal += ExtractAmount;
        
        mCurrentFluidBufferAmount -= ExtractAmount;
        mCurrentFluidBufferAmount = FMath::Clamp(mCurrentFluidBufferAmount, 0.0f, mFluidBufferToLoad);
    }
    else
    {
        // Item is still being burnt
        mCurrentEnergyValue -= 0.0166666f;
        
        mCurrentEnergyValue = FMath::Clamp(mCurrentEnergyValue, 0.0f, mMaxEnergyValue);
        //SML::Logging::info(mCurrentEnergyValue);
        addHeat(0.0166666f);
        
        ProduceCo2();
    }
}

// Fluid Buffer

bool ARPMPSolutionHeaterBuilding::CanTransferToFluidBuffer()
{
    FInventoryStack InputSteamItemStack;
    GetMPInventory()->GetStackFromIndex(mInputInvIndex, InputSteamItemStack);
    int InputSteamCount = InputSteamItemStack.NumItems;

    if (InputSteamCount > mFluidBufferToLoad)
    {
        return (mCurrentFluidBufferAmount == 0);
    }
    return false;
}

void ARPMPSolutionHeaterBuilding::TransferToFluidBuffer()
{
    if (CanTransferToFluidBuffer())
    {
        GetMPInventory()->RemoveFromIndex(mInputInvIndex, mFluidBufferToLoad);
        mCurrentFluidBufferAmount = mFluidBufferToLoad;
    }
}