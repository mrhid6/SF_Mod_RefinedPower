// ILikeBanas


#include "RPMPTurbineBuilding.h"


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

    TransferToFluidBuffer();

    if (CanStartSteamConsumption())
    {
        ConvertSteamToRPM();
    }
    else
    {
        mSteamConsumptionRate = 0;
    }

    ReduceRPM();

    int NewRPM = mCurrentTurbineRPM;
    mRPMRate = NewRPM - PrevRPM;

    if (mRPMRate != 0)
    {
        TriggerTurbineStateUpdated = true;
    }
}

bool ARPMPTurbineBuilding::CanStartSteamConsumption()
{
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
    // New Shiney
    float ExtractAmount = (float)FMath::Min(mCurrentFluidBufferAmount, mAmountFromBufferToTake);

    mCurrentFluidBufferAmount -= ExtractAmount;
    mCurrentFluidBufferAmount = FMath::Clamp(mCurrentFluidBufferAmount, 0, mFluidBufferToLoad);


    // Old Shit

    float ItemEnergyValue = UFGItemDescriptor::GetEnergyValue(mHighSteamItemClass);

    mSteamConsumptionRate = ((ExtractAmount * 60.0f)) / 1000.0f;

    float OutputAmount = ExtractAmount;
    OutputAmount += (ExtractAmount * mSteamDiscardPercent);

    ExtractAmount -= (ExtractAmount * mSteamDiscardPercent);

    OutputAmount /= 2;

    mSteamOutputRate = ((OutputAmount * 60)) / 1000;


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
    FInventoryStack InputSteamItemStack;
    GetMPInventory()->GetStackFromIndex(mInputInvIndex, InputSteamItemStack);
    int InputSteamCount = InputSteamItemStack.NumItems;

    if (InputSteamCount > mFluidBufferToLoad)
    {
        return (mCurrentFluidBufferAmount == 0);
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
