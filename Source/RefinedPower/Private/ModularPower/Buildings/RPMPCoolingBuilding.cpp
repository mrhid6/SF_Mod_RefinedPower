// ILikeBanas

#include "ModularPower/Buildings/RPMPCoolingBuilding.h"

ARPMPCoolingBuilding::ARPMPCoolingBuilding()
{
    SetReplicates(true);
    bReplicates = true;
    mFactoryTickFunction.SetTickFunctionEnable(true);
    mFactoryTickFunction.bCanEverTick = true;
}

void ARPMPCoolingBuilding::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        GetMPInventory()->SetDefaultSize(1);
        CacheConnections();
        
    }
}

void ARPMPCoolingBuilding::Factory_Tick(float dt)
{
    Super::Factory_Tick(dt);
    if (HasAuthority())
    {
        CollectItems(dt);
        TransferToFluidBuffer();
        
        if (CanEjectGas())
        {
            EjectGas();
        }
    }
}

void ARPMPCoolingBuilding::Tick(float dt)
{
    Super::Tick(dt);
}

void ARPMPCoolingBuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void ARPMPCoolingBuilding::CacheConnections()
{
    TArray<UActorComponent*> tempComps;

    FName tag = FName(TEXT("GasInputPipe"));
    tempComps = GetComponentsByTag(UFGPipeConnectionComponent::StaticClass(), tag);
    if (tempComps.Num() > 0)
    {
        InputPipe = Cast<UFGPipeConnectionComponent>(tempComps[0]);
    }
}

void ARPMPCoolingBuilding::CollectItems(float dt)
{
    if (InputPipe)
    {
        if (CanStoreItemInInventory(GetMPInventory(), mInputInvIndex, mInputItemClass, mInputPullAmount))
        {
            FInventoryStack outStack;
            bool pulledItem = InputPipe->Factory_PullPipeInput(dt, outStack, mInputItemClass, mInputPullAmount);

            if (pulledItem)
            {
                StoreItemStackInInventory(GetMPInventory(), mInputInvIndex, outStack);
            }
        }
    }
}

bool ARPMPCoolingBuilding::CanEjectGas()
{
    if (mCurrentFluidBufferAmount > 0.0f)
    {
        return true;
    }

    return false;
}

void ARPMPCoolingBuilding::EjectGas()
{
    float ExtractAmount = (float)FMath::Min(mCurrentFluidBufferAmount, mAmountFromBufferToTake);

    mCurrentFluidBufferAmount -= ExtractAmount;
    mCurrentFluidBufferAmount = FMath::Clamp(mCurrentFluidBufferAmount, 0.0f, mFluidBufferToLoad);

    mConsumptionTotal += ExtractAmount;
    
    mCurrentFluidBufferAmount -= ExtractAmount;

    mCurrentFluidBufferAmount = FMath::Clamp(mCurrentFluidBufferAmount, 0.0f, mFluidBufferToLoad);
}

// Fluid Buffer

bool ARPMPCoolingBuilding::CanTransferToFluidBuffer()
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

void ARPMPCoolingBuilding::TransferToFluidBuffer()
{
    if (CanTransferToFluidBuffer())
    {
        GetMPInventory()->RemoveFromIndex(mInputInvIndex, mFluidBufferToLoad);
        mCurrentFluidBufferAmount = mFluidBufferToLoad;
    }
}


bool ARPMPCoolingBuilding::CheckMPBuildingRunningState()
{

    FInventoryStack InputSteamItemStack;
    GetMPInventory()->GetStackFromIndex(mInputInvIndex, InputSteamItemStack);
    int InputSteamCount = InputSteamItemStack.NumItems;
    
    if(InputSteamCount > mFluidBufferToLoad)
    {
        return true;
    }else if(mCurrentFluidBufferAmount > 0)
    {
        return true;
    }

    return false;
}