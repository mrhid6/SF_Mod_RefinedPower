// ILikeBanas


#include "RPMPSolutionHeaterBuilding.h"

ARPMPSolutionHeaterBuilding::ARPMPSolutionHeaterBuilding()
{
    SetReplicates(true);
    bReplicates = true;
    mFactoryTickFunction.SetTickFunctionEnable(true);
    mFactoryTickFunction.bCanEverTick = true;
}

void ARPMPSolutionHeaterBuilding::Factory_Tick(float dt)
{
    Super::Factory_Tick(dt);

    if (HasAuthority())
    {
        CollectItems(dt);
        OutputCo2(dt);

        TransferToFluidBuffer();

        if (CanStartItemBurn())
        {
            BurnItem(dt);
        }
        else
        {
            subtractHeat(0.0166f);
        }
    }
}

bool ARPMPSolutionHeaterBuilding::CanStartItemBurn()
{
    if ((mCurrentFluidBufferAmount > 0 || mCurrentEnergyValue > 0) && getCo2ItemCount() < 50000)
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
        mCurrentEnergyValue = UFGItemDescriptor::GetEnergyValue(FuelItemStack.Item.ItemClass) * 1.5f / 60;
        mCurrentEnergyValue *= ExtractAmount;
        mCurrentEnergyValue *= mEnergyValueMultiplier;

        mMaxEnergyValue = mCurrentEnergyValue;

        // Remove fluid from buffer

        mLiquidFuelConsumptionRate = (ExtractAmount * 60.0f) / 1000.0f;
        
        mCurrentFluidBufferAmount -= ExtractAmount;
        mCurrentFluidBufferAmount = FMath::Clamp(mCurrentFluidBufferAmount, 0.0f, mFluidBufferToLoad);
    }
    else
    {
        // Item is still being burnt
        mCurrentEnergyValue -= (1.0f * dt);

        mCurrentEnergyValue = FMath::Clamp(mCurrentEnergyValue, 0.0f, mMaxEnergyValue);
        addHeat(0.0166f);
        
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

