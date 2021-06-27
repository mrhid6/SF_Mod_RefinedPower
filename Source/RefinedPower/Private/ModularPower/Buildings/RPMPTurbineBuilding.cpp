// ILikeBanas

#include "ModularPower/Buildings/RPMPTurbineBuilding.h"
#include "FGPlayerController.h"
#include "Logging.h"

void URPMPTurbineBuildingRCO::SetSteamDiscard_Implementation(ARPMPTurbineBuilding* panel, float value)
{
    panel->mSteamDiscardPercent = value;
    //panel->panelStateUpdated();

    panel->ForceNetUpdate();
}

bool URPMPTurbineBuildingRCO::SetSteamDiscard_Validate(ARPMPTurbineBuilding* panel, float value)
{
    return true;
}

void URPMPTurbineBuildingRCO::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(URPMPTurbineBuildingRCO, bTest);
}

ARPMPTurbineBuilding::ARPMPTurbineBuilding()
{
    SetReplicates(true);
    bReplicates = true;
    mFactoryTickFunction.SetTickFunctionEnable(true);
    mFactoryTickFunction.bCanEverTick = true;
}

void ARPMPTurbineBuilding::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        CacheConnections();
    }

    OnRep_TurbineStateUpdated();
}

void ARPMPTurbineBuilding::Factory_Tick(float dt)
{
    Super::Factory_Tick(dt);

    if (HasAuthority())
    {        
        CollectSteam(dt);
        OutputSteam(dt);
        CalcTurbineState();
    }
}

void ARPMPTurbineBuilding::Tick(float dt)
{
    Super::Tick(dt);
    if (HasAuthority())
    {
        if (TriggerTurbineStateUpdated)
        {
            TriggerTurbineStateUpdated = false;
            Multicast_TurbineStateUpdated();
        }
    }
}

void ARPMPTurbineBuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARPMPTurbineBuilding, mCurrentTurbineRPM)
    DOREPLIFETIME(ARPMPTurbineBuilding, mSteamDiscardPercent)
}


void ARPMPTurbineBuilding::CacheConnections()
{
    TArray<UActorComponent*> tempComps;

    FName tag = FName(TEXT("SteamInput"));
    tempComps = GetComponentsByTag(UFGPipeConnectionComponent::StaticClass(), tag);
    if (tempComps.Num() > 0)
    {
        InputSteamPipe = Cast<UFGPipeConnectionComponent>(tempComps[0]);
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
        OutputSteamPipe->SetInventoryAccessIndex(mOutputInvIndex);
    }
}

void ARPMPTurbineBuilding::CollectSteam(float dt)
{
    if (!IsValid(GetMPInventory()))
    {
        UE_LOG(LogRP, Error, TEXT("[RP] - ARPMPTurbineBuilding::CollectSteam | Inventory was null!"));
        return;
    }

    // UE_LOG(LogRP, Verbose, TEXT("[RP] - Retrieved inventory"));
    
    if (InputSteamPipe)
    {
        if (CanStoreItemInInventory(GetMPInventory(), mInputInvIndex, mHighSteamItemClass, mSteamPullAmount))
        {
            FInventoryStack outStack;
            bool pulledItem = InputSteamPipe->
                Factory_PullPipeInput(dt, outStack, mHighSteamItemClass, mSteamPullAmount);

            if (pulledItem)
            {
                StoreItemStackInInventory(GetMPInventory(), mInputInvIndex, outStack);
            }
        }
    }
}

void ARPMPTurbineBuilding::OutputSteam(float dt)
{
    if (!IsValid(GetMPInventory()))
    {
        UE_LOG(LogRP, Error, TEXT("[RP] - ARPMPTurbineBuilding::OutputSteam | Inventory was null!"));
        return;
    }

    if (GetMPInventory()->GetSizeLinear() == 0)
        return;
    // UE_LOG(LogRP, Verbose, TEXT("[RP] - Retrieved inventory"));
    
    FInventoryStack steamItemStack;    
    GetMPInventory()->GetStackFromIndex(mOutputInvIndex, steamItemStack);

    if (OutputSteamPipe && OutputSteamPipe->IsConnected())
    {
        FInventoryStack outStack;
        int32 TakenAmount = OutputSteamPipe->Factory_PushPipeOutput(dt, steamItemStack);

        if (steamItemStack.HasItems() && TakenAmount > 0)
        {
            GetMPInventory()->RemoveFromIndex(mOutputInvIndex, TakenAmount);
        }
    }
}


void ARPMPTurbineBuilding::CalcTurbineState()
{
    int PrevRPM = mCurrentTurbineRPM;

    ReduceRPM();
    
    TransferToFluidBuffer();

    if (CanStartSteamConsumption())
    {
        ConvertSteamToRPM();
    }

    int NewRPM = mCurrentTurbineRPM;
    mRPMRate = NewRPM - PrevRPM;

    if (mRPMRate != 0)
    {
        TriggerTurbineStateUpdated = true;
    }
}

bool ARPMPTurbineBuilding::CanStartSteamConsumption()
{
    if (!IsValid(GetMPInventory()))
    {
        UE_LOG(LogRP, Error, TEXT("[RP] - ARPMPTurbineBuilding::CanStartSteamConsumption | Inventory was null!"));
        return false;
    }
    if (GetMPInventory()->GetSizeLinear() == 0)
        return false;
    // UE_LOG(LogRP, Verbose, TEXT("[RP] - Retrieved inventory"));
    
    FInventoryStack OutputSteamItemStack;
    GetMPInventory()->GetStackFromIndex(mOutputInvIndex, OutputSteamItemStack);
    int OutputSteamCount = OutputSteamItemStack.NumItems;
    int MaxOutputCount = UFGItemDescriptor::GetStackSize(mLowSteamItemClass);

    //SML::Logging::info(OutputSteamCount);
    //SML::Logging::info(MaxOutputCount);

    return (mCurrentFluidBufferAmount > 0 && OutputSteamCount < MaxOutputCount);
}

void ARPMPTurbineBuilding::ConvertSteamToRPM()
{
    if (!IsValid(GetMPInventory()))
    {
        UE_LOG(LogRP, Error, TEXT("[RP] - ARPMPTurbineBuilding::ConvertSteamToRPM | Inventory was null!"));
        return;
    }
    if (GetMPInventory()->GetSizeLinear() == 0)
        return;
    // UE_LOG(LogRP, Verbose, TEXT("[RP] - Retrieved inventory"));
    
    
    // New Shiney
    float ExtractAmount = (float)FMath::Min(mCurrentFluidBufferAmount, mAmountFromBufferToTake);

    mCurrentFluidBufferAmount -= ExtractAmount;
    mCurrentFluidBufferAmount = FMath::Clamp(mCurrentFluidBufferAmount, 0.0f, mFluidBufferToLoad);


    // Old Shit
    float ItemEnergyValue = UFGItemDescriptor::GetEnergyValue(mHighSteamItemClass);

    mConsumptionTotal += ExtractAmount;

    float OutputAmount = ExtractAmount;
    OutputAmount += (ExtractAmount * mSteamDiscardPercent);

    ExtractAmount -= (ExtractAmount * mSteamDiscardPercent);

    OutputAmount /= 2;
    mProductionTotal += OutputAmount;


    float EnergyValue = (ItemEnergyValue * ExtractAmount);
    EnergyValue = FMath::Clamp(EnergyValue, 0.0f, 9999999.0f);

    //SML::Logging::info("[RefinedPower] - EnergyValue: ", EnergyValue);

    int RPMToAdd = FMath::FloorToInt((EnergyValue / 3) * mRPMMultiplier);

    mCurrentTurbineRPM += RPMToAdd;
    mCurrentTurbineRPM = FMath::Clamp(mCurrentTurbineRPM, 0, mRedMaxTurbineRPM);

    StoreItemInInventory(GetMPInventory(), mOutputInvIndex, mLowSteamItemClass, OutputAmount);
}

void ARPMPTurbineBuilding::ReduceRPM()
{
    if (mCurrentTurbineRPM > 0)
    {
        mCurrentTurbineRPM -= mRPMDrag;
        mCurrentTurbineRPM = FMath::Clamp(mCurrentTurbineRPM, 0, mRedMaxTurbineRPM);
    }
}

void ARPMPTurbineBuilding::Multicast_TurbineStateUpdated_Implementation()
{
    OnRep_TurbineStateUpdated();
}

// RCO Stuff

void ARPMPTurbineBuilding::SetSteamDiscard(float value)
{
    auto rco = Cast<URPMPTurbineBuildingRCO>(
        Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(
            URPMPTurbineBuildingRCO::StaticClass()));

    if (rco)
    {
        rco->SetSteamDiscard(this, value);
    }
}


// Fluid Buffer

bool ARPMPTurbineBuilding::CanTransferToFluidBuffer()
{
    if (!IsValid(GetMPInventory()))
    {
        UE_LOG(LogRP, Error, TEXT("[RP] - ARPMPTurbineBuilding::CanTransferToFluidBuffer | Inventory was null!"));
        return false;
    }
    if (GetMPInventory()->GetSizeLinear() == 0)
        return false;
    // UE_LOG(LogRP, Verbose, TEXT("[RP] - Retrieved inventory"));
    
    FInventoryStack InputSteamItemStack;
    
    GetMPInventory()->GetStackFromIndex(mInputInvIndex, InputSteamItemStack);
    const int InputSteamCount = InputSteamItemStack.NumItems;

    if (InputSteamCount > mFluidBufferToLoad)
    {
        return mCurrentFluidBufferAmount == 0;
    }
    return false;
}

void ARPMPTurbineBuilding::TransferToFluidBuffer()
{
    if (CanTransferToFluidBuffer())
    {
        GetMPInventory()->RemoveFromIndex(mInputInvIndex, mFluidBufferToLoad);
        mCurrentFluidBufferAmount = mFluidBufferToLoad;
    }
}


// FIN Intergration

int ARPMPTurbineBuilding::netFunc_getCurrentRPM()
{
    return mCurrentTurbineRPM;
}

float ARPMPTurbineBuilding::netFunc_getSteamDiscardPercent()
{
    return mSteamDiscardPercent;
}