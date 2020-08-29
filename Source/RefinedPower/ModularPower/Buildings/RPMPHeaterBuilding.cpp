// ILikeBanas


#include "RPMPHeaterBuilding.h"

ARPMPHeaterBuilding::ARPMPHeaterBuilding()
{
    SetReplicates(true);
    bReplicates = true;
    mFactoryTickFunction.SetTickFunctionEnable(true);
    mFactoryTickFunction.bCanEverTick = true;
}

void ARPMPHeaterBuilding::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        GetMPInventory()->mItemFilter.BindUFunction(this, "FilterFuelClasses");
        CacheConnections();
        FTimerHandle timerHandle;
        GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ARPMPHeaterBuilding::UpdateCo2ProductionRate,
                                           60.0f, true);
    }
}

void ARPMPHeaterBuilding::Factory_Tick(float dt)
{
    Super::Factory_Tick(dt);
    if (HasAuthority())
    {
        CollectItems(dt);
        OutputCo2(dt);

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

void ARPMPHeaterBuilding::Tick(float dt)
{
    Super::Tick(dt);
}

void ARPMPHeaterBuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARPMPHeaterBuilding, mCurrentHeatValue)
    DOREPLIFETIME(ARPMPHeaterBuilding, mCurrentEnergyValue)
    DOREPLIFETIME(ARPMPHeaterBuilding, mMaxEnergyValue)
    DOREPLIFETIME(ARPMPHeaterBuilding, mLiquidFuelConsumptionRate)
    DOREPLIFETIME(ARPMPHeaterBuilding, mCo2ProductionRate)
}


void ARPMPHeaterBuilding::CacheConnections()
{
    TArray<UActorComponent*> tempComps;

    FName tag = FName(TEXT("FuelConnection"));
    tempComps = GetComponentsByTag(UFGFactoryConnectionComponent::StaticClass(), tag);
    if (tempComps.Num() > 0)
    {
        InputFuelConveyor = Cast<UFGFactoryConnectionComponent>(tempComps[0]);
    }

    tag = FName(TEXT("WasteConnection"));
    tempComps = GetComponentsByTag(UFGFactoryConnectionComponent::StaticClass(), tag);
    if (tempComps.Num() > 0)
    {
        OutputWasteConveyor = Cast<UFGFactoryConnectionComponent>(tempComps[0]);
        OutputWasteConveyor->SetInventory(GetMPInventory());
        OutputWasteConveyor->SetInventoryAccessIndex(mOutputInvIndex);
    }

    tag = FName(TEXT("FuelPipe"));
    tempComps = GetComponentsByTag(UFGPipeConnectionComponent::StaticClass(), tag);
    if (tempComps.Num() > 0)
    {
        InputFuelPipe = Cast<UFGPipeConnectionComponent>(tempComps[0]);
    }

    tag = FName(TEXT("Co2Pipe"));
    tempComps = GetComponentsByTag(UFGPipeConnectionComponent::StaticClass(), tag);
    if (tempComps.Num() > 0)
    {
        OutputCo2Pipe = Cast<UFGPipeConnectionComponent>(tempComps[0]);

        OutputCo2Pipe->SetInventory(GetMPInventory());
        OutputCo2Pipe->SetInventoryAccessIndex(mOutputInvIndex);
    }

    if (OutputCo2Pipe == nullptr)
    {
        mProducesCo2 = false;
    }

    if (OutputWasteConveyor == nullptr)
    {
        mProducesNukeWaste = false;
    }
}

void ARPMPHeaterBuilding::CollectItems(float dt)
{
    if (InputFuelConveyor)
    {
        for (auto AllowedItem : mAllowedFuelItems)
        {
            if (CanStoreItemInInventory(GetMPInventory(), mInputInvIndex, AllowedItem, 1))
            {
                FInventoryItem outItem;
                float offset = 0.0f;
                bool pulledItem = InputFuelConveyor->Factory_GrabOutput(outItem, offset, AllowedItem);
                if (pulledItem)
                {
                    StoreItemInInventory(GetMPInventory(), mInputInvIndex, AllowedItem, 1);
                }
            }
        }
    }

    if (InputFuelPipe)
    {
        for (auto AllowedItem : mAllowedFuelItems)
        {
            if (CanStoreItemInInventory(GetMPInventory(), mInputInvIndex, AllowedItem, 100))
            {
                FInventoryStack outStack;
                bool pulledItem = InputFuelPipe->Factory_PullPipeInput(dt, outStack, AllowedItem, 100);
                if (pulledItem)
                {
                    StoreItemInInventory(GetMPInventory(), mInputInvIndex, AllowedItem, 100);
                }
            }
        }
    }
}

int ARPMPHeaterBuilding::getFuelItemCount()
{
    FInventoryStack out_stack;
    GetMPInventory()->GetStackFromIndex(mInputInvIndex, out_stack);
    if (out_stack.HasItems())
    {
        return out_stack.NumItems;
    }

    return 0;
}

int ARPMPHeaterBuilding::getCo2ItemCount()
{
    FInventoryStack out_stack;
    GetMPInventory()->GetStackFromIndex(mOutputInvIndex, out_stack);
    if (out_stack.HasItems())
    {
        return out_stack.NumItems;
    }

    return 0;
}

bool ARPMPHeaterBuilding::CanStartItemBurn()
{
    if ((getFuelItemCount() > 0 || mCurrentEnergyValue > 0) && getCo2ItemCount() < 50000)
    {
        return (mCurrentHeatValue < mMaxHeatValue);
    }

    return false;
}

void ARPMPHeaterBuilding::BurnItem(float dt)
{
    if (mCurrentEnergyValue <= 0)
    {
        // No Items Being Burnt
        FInventoryStack FuelItemStack;
        GetMPInventory()->GetStackFromIndex(mInputInvIndex, FuelItemStack);

        // Biomass - 4.5 Seconds;
        mCurrentEnergyValue = UFGItemDescriptor::GetEnergyValue(FuelItemStack.Item.ItemClass) * 1.5f / 60;
        mCurrentEnergyValue *= mEnergyValueMultiplier;

        //SML::Logging::info(mCurrentEnergyValue);

        mMaxEnergyValue = mCurrentEnergyValue;

        GetMPInventory()->RemoveFromIndex(mInputInvIndex, 1);

        ProduceNukeWaste();
    }
    else
    {
        mCurrentEnergyValue -= (1.0f * dt);

        mCurrentEnergyValue = FMath::Clamp(mCurrentEnergyValue, 0.0f, mMaxEnergyValue);
        addHeat(0.0166f);
        
        ProduceCo2();
    }
}

void ARPMPHeaterBuilding::ProduceCo2()
{
    if (mProducesCo2)
    {
        if (CanStoreItemInInventory(GetMPInventory(), mOutputInvIndex, mCo2ItemClass, mOutputGenerationAmount))
        {
            StoreItemInInventory(GetMPInventory(), mOutputInvIndex, mCo2ItemClass, mOutputGenerationAmount);

            mCo2TotalProductionAmount += mOutputGenerationAmount;
        }
    }
}

void ARPMPHeaterBuilding::ProduceNukeWaste()
{
    if (mProducesNukeWaste)
    {
        if (CanStoreItemInInventory(GetMPInventory(), mOutputInvIndex, mNukeWasteItemClass, mOutputGenerationAmount))
        {
            StoreItemInInventory(GetMPInventory(), mOutputInvIndex, mNukeWasteItemClass, mOutputGenerationAmount);
        }
    }
}

void ARPMPHeaterBuilding::OutputCo2(float dt)
{
    FInventoryStack co2ItemStack;
    GetMPInventory()->GetStackFromIndex(mOutputInvIndex, co2ItemStack);
    int co2ItemCount = co2ItemStack.NumItems;

    if (co2ItemCount >= mOutputGenerationAmount)
    {
        co2ItemStack.NumItems = mOutputGenerationAmount;
    }

    if (OutputCo2Pipe && OutputCo2Pipe->IsConnected())
    {
        FInventoryStack outStack;
        int32 TakenAmount = OutputCo2Pipe->Factory_PushPipeOutput(dt, co2ItemStack);

        if (co2ItemStack.HasItems() && TakenAmount > 0)
        {
            GetMPInventory()->RemoveFromIndex(mOutputInvIndex, TakenAmount);
        }
    }
}


bool ARPMPHeaterBuilding::FilterFuelClasses( TSubclassOf< UObject > object, int32 idx ) const
{
    for (TSubclassOf<UFGItemDescriptor> tempItem: mAllowedFuelItems)
    {
        if(tempItem == object)
        {
            return true;
        }
    }
    
    if(mCo2ItemClass == object || mNukeWasteItemClass == object)
    {
        return true;
    }

    return false;
}

bool ARPMPHeaterBuilding::CheckMPBuildingRunningState()
{
    if(getFuelItemCount() > 0 && mCurrentFluidBufferAmount > 0)
    {
        return true;
    }else if(mCurrentFluidBufferAmount > 0)
    {
        return true;
    }else if(mCurrentHeatValue > 0)
    {
        return true;
    }

    return false;
}


void ARPMPHeaterBuilding::UpdateCo2ProductionRate()
{
    mCo2ProductionRate = mCo2TotalProductionAmount / 60.0f / 60.0f / 60.0f;
    mCo2TotalProductionAmount = 0.0f;
}