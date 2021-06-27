#include "RefinedPower/Public/DysonSphere/RPDysonSphere.h"
#include "FGFactoryConnectionComponent.h"
#include "FGPipeConnectionComponent.h"
#include "FGGameState.h"
#include "FGPowerInfoComponent.h"
#include "FGPowerConnectionComponent.h"
#include "FGInventoryComponent.h"
#include "FGPlayerController.h"
#include "FGSchematicManager.h"
#include "FGSchematic.h"

void URPDysonSphereRCO::ResetFailedDysonSphere_Implementation(ARPDysonSphere* ds)
{
    ds->mDSLightBeamState = EDSLightBeamState::RP_DSLB_State_Grow;
    ds->mDSBuildState = EDSBuildState::RP_DSB_State_0;
    ds->mDysonSphereState = EDysonSphereState::RP_DS_State_Build;
    ds->mRepairTimer = 0;
    ds->mFailedRepairs = 0;
    ds->CalcUnlockedHangars();
    ds->ClearBuildStageItemCount();
    ds->ClearRepairItemCount();
    ds->ResetLightBeam();

    ds->ForceNetUpdate();
}

bool URPDysonSphereRCO::ResetFailedDysonSphere_Validate(ARPDysonSphere* ds)
{
    return true;
}

void URPDysonSphereRCO::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(URPDysonSphereRCO, bTest)
}


ARPDysonSphere::ARPDysonSphere()
{
    FGPowerConnection = CreateDefaultSubobject<UFGPowerConnectionComponent>(TEXT("FGPowerConnection"));
    FGPowerConnection->SetupAttachment(RootComponent);

    H1Input1 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H1Input1"));
    H1Input2 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H1Input2"));
    H1Input3 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H1Input3"));
    H1Input4 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H1Input4"));
    H1Input5 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H1Input5"));
    H1Input6 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H1Input6"));

    H1Input1->SetupAttachment(RootComponent);
    H1Input2->SetupAttachment(RootComponent);
    H1Input3->SetupAttachment(RootComponent);
    H1Input4->SetupAttachment(RootComponent);
    H1Input5->SetupAttachment(RootComponent);
    H1Input6->SetupAttachment(RootComponent);

    H2Input1 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H2Input1"));
    H2Input2 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H2Input2"));
    H2Input3 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H2Input3"));
    H2Input4 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H2Input4"));
    H2Input5 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H2Input5"));
    H2Input6 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H2Input6"));

    H2Input1->SetupAttachment(RootComponent);
    H2Input2->SetupAttachment(RootComponent);
    H2Input3->SetupAttachment(RootComponent);
    H2Input4->SetupAttachment(RootComponent);
    H2Input5->SetupAttachment(RootComponent);
    H2Input6->SetupAttachment(RootComponent);

    H3Input1 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H3Input1"));
    H3Input2 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H3Input2"));
    H3Input3 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H3Input3"));
    H3Input4 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H3Input4"));
    H3Input5 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H3Input5"));
    H3Input6 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H3Input6"));

    H3Input1->SetupAttachment(RootComponent);
    H3Input2->SetupAttachment(RootComponent);
    H3Input3->SetupAttachment(RootComponent);
    H3Input4->SetupAttachment(RootComponent);
    H3Input5->SetupAttachment(RootComponent);
    H3Input6->SetupAttachment(RootComponent);

    H4Input1 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H4Input1"));
    H4Input2 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H4Input2"));
    H4Input3 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H4Input3"));
    H4Input4 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H4Input4"));
    H4Input5 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H4Input5"));
    H4Input6 = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("H4Input6"));

    H4Input1->SetupAttachment(RootComponent);
    H4Input2->SetupAttachment(RootComponent);
    H4Input3->SetupAttachment(RootComponent);
    H4Input4->SetupAttachment(RootComponent);
    H4Input5->SetupAttachment(RootComponent);
    H4Input6->SetupAttachment(RootComponent);


    SetReplicates(true);
    bReplicates = true;
    mFactoryTickFunction.bCanEverTick = true;
}

ARPDysonSphere::~ARPDysonSphere()
{
}

void ARPDysonSphere::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ARPDysonSphere, mDysonSphereState);
    DOREPLIFETIME(ARPDysonSphere, mDSBuildState);
    DOREPLIFETIME(ARPDysonSphere, mDSLightBeamState);


    DOREPLIFETIME(ARPDysonSphere, mHangar1Enabled);
    DOREPLIFETIME(ARPDysonSphere, mHangar2Enabled);
    DOREPLIFETIME(ARPDysonSphere, mHangar3Enabled);
    DOREPLIFETIME(ARPDysonSphere, mHangar4Enabled);


    // Dont think i need these?
    //DOREPLIFETIME(ARPDysonSphere, mTriggerHangarLightUpdate);
    //DOREPLIFETIME(ARPDysonSphere, mTriggerStartShipAnimation);
    //DOREPLIFETIME(ARPDysonSphere, mTriggerLightBeamUpdate);

    DOREPLIFETIME(ARPDysonSphere, mLightBeamScale);
    DOREPLIFETIME(ARPDysonSphere, mLightBeamVisiable);


    DOREPLIFETIME(ARPDysonSphere, mBuildStageItemCount);

    DOREPLIFETIME(ARPDysonSphere, mRepairItemCount);
    DOREPLIFETIME(ARPDysonSphere, mRepairTimer);
    DOREPLIFETIME(ARPDysonSphere, mFailedRepairs);
}

void ARPDysonSphere::BeginPlay()
{
    Super::BeginPlay();

    mBuildStageItemCount.SetNumZeroed(mAllowedInputItems.Num());
    mRepairItemCount.SetNumZeroed(mAllowedInputItems.Num());

    mLightBeamScale = FVector(0.0f, 0.0f, 20.0f);
    mLightBeamVisiable = false;

    if (HasAuthority())
    {
        FGPowerConnection->SetPowerInfo(GetPowerInfo());
        CalcUnlockedHangars();
        SetPowerOutput();
    }
}

void ARPDysonSphere::Tick(float dt)
{
    Super::Tick(dt);

    if (HasAuthority())
    {
        if (mTriggerHangarLightEvent)
        {
            mTriggerHangarLightEvent = false;
            Multicast_UpdateHangarLights();
        }

        if (mTriggerShipAnimEvent)
        {
            mTriggerShipAnimEvent = false;
            Multicast_StartShipAnimation();
        }

        if (mTriggerLightBeamEvent)
        {
            mTriggerLightBeamEvent = false;
            Multicast_UpdateLightBeam();
        }
    }
}

void ARPDysonSphere::Factory_Tick(float dt)
{
    Super::Factory_Tick(dt);

    if (HasAuthority())
    {
        CalcDysonSphereState(dt);
    }

    if (HasAuthority() && mDoorAnimationTimer > 0)
    {
        mDoorAnimationTimer--;
        mDoorAnimationTimer = FMath::Clamp(mDoorAnimationTimer, 0, mDoorAnimationTimerDuration);
    }
}

void ARPDysonSphere::CalcUnlockedHangars()
{
    AFGSchematicManager* schemMgr = AFGSchematicManager::Get(GetWorld());

    TArray<TSubclassOf<UFGSchematic>> out_schematics;

    schemMgr->GetAllPurchasedSchematics(out_schematics);

    mHangar1Enabled = false;
    mHangar2Enabled = false;
    mHangar3Enabled = false;
    mHangar4Enabled = false;


    for (TSubclassOf<UFGSchematic> schem : out_schematics)
    {
        FText SchemName = UFGSchematic::GetSchematicDisplayName(schem);

        if (SchemName.ToString() == "Dyson Sphere H1")
        {
            //SML::Logging::info("[RefinedPower] - Hangar 1 Unlocked");
            mHangar1Enabled = true;
        }

        if (SchemName.ToString() == "Dyson Sphere H2")
        {
            //SML::Logging::info("[RefinedPower] - Hangar 2 Unlocked");
            mHangar2Enabled = true;
        }

        if (SchemName.ToString() == "Dyson Sphere H3")
        {
            //SML::Logging::info("[RefinedPower] - Hangar 3 Unlocked");
            mHangar3Enabled = true;
        }

        if (SchemName.ToString() == "Dyson Sphere H4")
        {
            //SML::Logging::info("[RefinedPower] - Hangar 4 Unlocked");
            mHangar4Enabled = true;
        }
    }
    
    mTriggerHangarLightEvent = true;
}

int ARPDysonSphere::GetUnlockedHangarCount()
{
    int resInt = 0;

    if (mHangar1Enabled) resInt += 1;
    if (mHangar2Enabled) resInt += 1;
    if (mHangar3Enabled) resInt += 1;
    if (mHangar4Enabled) resInt += 1;

    return resInt;
}

void ARPDysonSphere::CalcDysonSphereState(float dt)
{
    switch (mDysonSphereState)
    {
    case EDysonSphereState::RP_DS_State_Build:
        {
            CalcBuildState();
            break;
        }
    case EDysonSphereState::RP_DS_State_LightBeam:
        {
            CalcLightBeamState();
            break;
        }
    case EDysonSphereState::RP_DS_State_Producing:
        {
            CalcProducingState(dt);
            break;
        }
    default:
        break;
    }
}

void ARPDysonSphere::SetDysonSphereState(EDysonSphereState state)
{
    mDysonSphereState = state;
}

bool ARPDysonSphere::IsDoorAnimationFinished()
{
    if (mDoorAnimationTimer > 0)
    {
        return false;
    }

    return true;
}

void ARPDysonSphere::StartDoorAnimationTimer()
{
    mDoorAnimationTimer = mDoorAnimationTimerDuration;
}

float ARPDysonSphere::GetPowerOutput()
{
    float resOutput = 0;

    if (mDysonSphereState == EDysonSphereState::RP_DS_State_Producing)
    {
        float Base = GetUnlockedHangarCount() * mPowerProduction;
        float loss = mFailedRepairs * 50000.0f;

        resOutput = Base - loss;
        resOutput = FMath::Clamp(resOutput, 0.0f, Base);
    }

    return resOutput;
}

void ARPDysonSphere::SetPowerOutput()
{
    UFGPowerInfoComponent* TempFGPowerInfo = FGPowerConnection->GetPowerInfo();

    if (TempFGPowerInfo != nullptr)
    {
        TempFGPowerInfo->SetBaseProduction(GetPowerOutput());
    }
}

void ARPDysonSphere::CalcBuildState()
{
    switch (mDSBuildState)
    {
    case EDSBuildState::RP_DSB_State_0:
        {
            CollectBuildItems();

            if (IsBuildStageCompleted())
            {
                //SML::Logging::info("[RefinedPower] - BuildStage0 Completed!");
                mTriggerShipAnimEvent = true;
                StartDoorAnimationTimer();
                ClearBuildStageItemCount();
                CalcUnlockedHangars();
                SetBuildStageState(EDSBuildState::RP_DSB_State_1);
            }
            break;
        }
    case EDSBuildState::RP_DSB_State_1:
        {
            CollectBuildItems();

            if (IsBuildStageCompleted())
            {
                //SML::Logging::info("[RefinedPower] - BuildStage1 Completed!");
                mTriggerShipAnimEvent = true;
                StartDoorAnimationTimer();
                ClearBuildStageItemCount();
                CalcUnlockedHangars();
                SetBuildStageState(EDSBuildState::RP_DSB_State_2);
            }
            break;
        }
    case EDSBuildState::RP_DSB_State_2:
        {
            CollectBuildItems();

            if (IsBuildStageCompleted())
            {
                //SML::Logging::info("[RefinedPower] - BuildStage2 Completed!");
                mTriggerShipAnimEvent = true;
                StartDoorAnimationTimer();
                ClearBuildStageItemCount();
                CalcUnlockedHangars();
                SetBuildStageState(EDSBuildState::RP_DSB_State_3);
            }
            break;
        }
    case EDSBuildState::RP_DSB_State_3:
        {
            CollectBuildItems();

            if (IsBuildStageCompleted())
            {
                //SML::Logging::info("[RefinedPower] - BuildStage3 Completed!");
                mTriggerShipAnimEvent = true;
                StartDoorAnimationTimer();
                ClearBuildStageItemCount();
                CalcUnlockedHangars();
                SetBuildStageState(EDSBuildState::RP_DSB_State_Completed);
            }
            break;
        }
    case EDSBuildState::RP_DSB_State_Completed:
        {
            bool DoorAnimFinished = IsDoorAnimationFinished();
            if (DoorAnimFinished == false) return;

            //SML::Logging::info("[RefinedPower] - ALL BuildStage Completed!");
            CalcUnlockedHangars();
            ResetLightBeam();
            SetDysonSphereState(EDysonSphereState::RP_DS_State_LightBeam);
            break;
        }
    default:
        break;
    }
}

void ARPDysonSphere::SetBuildStageState(EDSBuildState state)
{
    mDSBuildState = state;
}

void ARPDysonSphere::CollectBuildItems()
{
    bool DoorAnimFinished = IsDoorAnimationFinished();
    if (DoorAnimFinished == false) return;

    int InputItemIndex = 0;

    // Loop through all allowed items
    for (TSubclassOf<UFGItemDescriptor>& InputItem : mAllowedInputItems)
    {
        bool CanCollectItem = CanTransferItemForBuildState(InputItemIndex);

        if (CanCollectItem == false)
        {
            InputItemIndex++;
            continue;
        };

        // Transfer all from Hangar1 Inputs
        TransferBuildStageItem(H1Input1, InputItem, InputItemIndex);
        TransferBuildStageItem(H1Input2, InputItem, InputItemIndex);
        TransferBuildStageItem(H1Input3, InputItem, InputItemIndex);
        TransferBuildStageItem(H1Input4, InputItem, InputItemIndex);
        TransferBuildStageItem(H1Input5, InputItem, InputItemIndex);
        TransferBuildStageItem(H1Input6, InputItem, InputItemIndex);

        // Transfer all from Hangar2 Inputs
        TransferBuildStageItem(H2Input1, InputItem, InputItemIndex);
        TransferBuildStageItem(H2Input2, InputItem, InputItemIndex);
        TransferBuildStageItem(H2Input3, InputItem, InputItemIndex);
        TransferBuildStageItem(H2Input4, InputItem, InputItemIndex);
        TransferBuildStageItem(H2Input5, InputItem, InputItemIndex);
        TransferBuildStageItem(H2Input6, InputItem, InputItemIndex);

        // Transfer all from Hangar3 Inputs
        TransferBuildStageItem(H3Input1, InputItem, InputItemIndex);
        TransferBuildStageItem(H3Input2, InputItem, InputItemIndex);
        TransferBuildStageItem(H3Input3, InputItem, InputItemIndex);
        TransferBuildStageItem(H3Input4, InputItem, InputItemIndex);
        TransferBuildStageItem(H3Input5, InputItem, InputItemIndex);
        TransferBuildStageItem(H3Input6, InputItem, InputItemIndex);

        // Transfer all from Hangar4 Inputs
        TransferBuildStageItem(H4Input1, InputItem, InputItemIndex);
        TransferBuildStageItem(H4Input2, InputItem, InputItemIndex);
        TransferBuildStageItem(H4Input3, InputItem, InputItemIndex);
        TransferBuildStageItem(H4Input4, InputItem, InputItemIndex);
        TransferBuildStageItem(H4Input5, InputItem, InputItemIndex);
        TransferBuildStageItem(H4Input6, InputItem, InputItemIndex);

        InputItemIndex++;
    }
}

bool ARPDysonSphere::CanTransferItemForBuildState(int AllowedItemIndex)
{
    int itemCount = mBuildStageItemCount[AllowedItemIndex];
    int maxAmount = mBuildStageMaxItemNum[AllowedItemIndex];
    if (itemCount < maxAmount)
    {
        return true;
    }

    return false;
}

void ARPDysonSphere::TransferBuildStageItem(UFGFactoryConnectionComponent* InputConnection,
                                            TSubclassOf<UFGItemDescriptor> InputItem, int InputItemIndex)
{
    FInventoryItem out_item;
    float out_Offsetbeyond;

    if (InputConnection->Factory_GrabOutput(out_item, out_Offsetbeyond, InputItem))
    {
        int max = mBuildStageMaxItemNum[InputItemIndex];
        int count = mBuildStageItemCount[InputItemIndex] + 1;

        mBuildStageItemCount[InputItemIndex] = FMath::Clamp(count, 0, max);
    }
}

bool ARPDysonSphere::IsBuildStageCompleted()
{
    int itemIndex = 0;

    bool completed = true;

    for (int ItemCount : mBuildStageItemCount)
    {
        int max = mBuildStageMaxItemNum[itemIndex];

        if (ItemCount < max)
        {
            completed = false;
        }

        itemIndex++;
    }

    return completed;
}

void ARPDysonSphere::ClearBuildStageItemCount()
{
    int itemIndex = 0;

    for (int ItemCount : mBuildStageItemCount)
    {
        mBuildStageItemCount[itemIndex] = 0;
        itemIndex++;
    }
}

void ARPDysonSphere::CalcLightBeamState()
{
    switch (mDSLightBeamState)
    {
    case EDSLightBeamState::RP_DSLB_State_Grow:
        {
            GrowLightBeam();

            if (IsLightBeamCompleted())
            {
                //SML::Logging::info("[RefinedPower] - LightBeam Completed!");
                SetLightBeamState(EDSLightBeamState::RP_DSLB_State_Completed);
            }
            break;
        }
    case EDSLightBeamState::RP_DSLB_State_Completed:
        {
            SetLightBeamFull();
            CalcUnlockedHangars();
            SetDysonSphereState(EDysonSphereState::RP_DS_State_Producing);
            SetPowerOutput();
            break;
        }
    }
}

void ARPDysonSphere::ResetLightBeam()
{
    mLightBeamScale = FVector(0.0f, 0.0f, 20.0f);
    mLightBeamVisiable = false;
    mTriggerLightBeamEvent = true;
}

void ARPDysonSphere::SetLightBeamFull()
{
    if (IsLightBeamCompleted() == false)
    {
        mLightBeamScale = FVector(1.0f, 1.0f, 20.0f);
        mLightBeamVisiable = true;
        mTriggerLightBeamEvent = true;
    }
}

void ARPDysonSphere::GrowLightBeam()
{
    mLightBeamVisiable = true;
    float temp = mLightBeamScale.X;
    temp += 0.0008333f;

    mLightBeamScale.X = FMath::Clamp(temp, 0.0f, 1.0f);
    mLightBeamScale.Y = FMath::Clamp(temp, 0.0f, 1.0f);

    mTriggerLightBeamEvent = true;
}

bool ARPDysonSphere::IsLightBeamCompleted()
{
    return (mLightBeamScale.X == 1.0f && mLightBeamScale.Y == 1.0f && mLightBeamScale.Z == 20.0f);
}

void ARPDysonSphere::SetLightBeamState(EDSLightBeamState state)
{
    mDSLightBeamState = state;
}

void ARPDysonSphere::CalcProducingState(float dt)
{
    SetLightBeamFull();

    if (IsRepairTimerFinished())
    {
        //SML::Logging::info("[RefinedPower] - Repair Timer Finished");
        ResetRepairTimer();

        if (IsRepairItemsCompleted())
        {
           // SML::Logging::info("[RefinedPower] - Repair Items Completed");
        }
        else
        {
           // SML::Logging::info("[RefinedPower] - Not Enough Repair Items");
            mFailedRepairs++;
            mFailedRepairs = FMath::Clamp(mFailedRepairs, 0, mMaxFailedRepairs);

            if (mFailedRepairs == mMaxFailedRepairs)
            {
                ResetLightBeam();
                SetDysonSphereState(EDysonSphereState::RP_DS_State_Failed);
            }
        }
        mTriggerShipAnimEvent = true;
        StartDoorAnimationTimer();
        CalcUnlockedHangars();
        ClearRepairItemCount();
        SetPowerOutput();
    }
    else
    {
        if (IsDoorAnimationFinished())
        {
            IncreaseRepairTimer(dt);
            CollectRepairItems();
        }
    }
}

void ARPDysonSphere::IncreaseRepairTimer(float dt)
{
    mRepairTimer += dt;
}

void ARPDysonSphere::ResetRepairTimer()
{
    mRepairTimer = 0;
}

bool ARPDysonSphere::IsRepairTimerFinished()
{
    if (mRepairTimer >= GetRepairTimerLimit())
    {
        return true;
    }

    return false;
}

float ARPDysonSphere::GetRepairTimerLimit()
{
    float RepairLimit = ((GetUnlockedHangarCount() * 1.5f) * 60.0f) + 210.0f;
    return RepairLimit;
}

bool ARPDysonSphere::IsRepairItemsCompleted()
{
    int itemIndex = 0;

    bool completed = true;

    for (int ItemCount : mRepairItemCount)
    {
        int max = mRepairMaxItemNum[itemIndex];

        if (ItemCount < max)
        {
            completed = false;
        }

        itemIndex++;
    }

    return completed;
}

void ARPDysonSphere::CollectRepairItems()
{
    bool DoorAnimFinished = IsDoorAnimationFinished();
    if (DoorAnimFinished == false) return;

    int InputItemIndex = 0;

    // Loop through all allowed items
    for (TSubclassOf<UFGItemDescriptor> InputItem : mAllowedInputItems)
    {
        bool CanCollectItem = CanTransferItemForRepair(InputItemIndex);

        if (CanCollectItem == false)
        {
            InputItemIndex++;
            continue;
        };

        // Transfer all from Hangar1 Inputs
        TransferRepairItem(H1Input1, InputItem, InputItemIndex);
        TransferRepairItem(H1Input2, InputItem, InputItemIndex);
        TransferRepairItem(H1Input3, InputItem, InputItemIndex);
        TransferRepairItem(H1Input4, InputItem, InputItemIndex);
        TransferRepairItem(H1Input5, InputItem, InputItemIndex);
        TransferRepairItem(H1Input6, InputItem, InputItemIndex);

        // Transfer all from Hangar2 Inputs
        TransferRepairItem(H2Input1, InputItem, InputItemIndex);
        TransferRepairItem(H2Input2, InputItem, InputItemIndex);
        TransferRepairItem(H2Input3, InputItem, InputItemIndex);
        TransferRepairItem(H2Input4, InputItem, InputItemIndex);
        TransferRepairItem(H2Input5, InputItem, InputItemIndex);
        TransferRepairItem(H2Input6, InputItem, InputItemIndex);

        // Transfer all from Hangar3 Inputs
        TransferRepairItem(H3Input1, InputItem, InputItemIndex);
        TransferRepairItem(H3Input2, InputItem, InputItemIndex);
        TransferRepairItem(H3Input3, InputItem, InputItemIndex);
        TransferRepairItem(H3Input4, InputItem, InputItemIndex);
        TransferRepairItem(H3Input5, InputItem, InputItemIndex);
        TransferRepairItem(H3Input6, InputItem, InputItemIndex);

        // Transfer all from Hangar4 Inputs
        TransferRepairItem(H4Input1, InputItem, InputItemIndex);
        TransferRepairItem(H4Input2, InputItem, InputItemIndex);
        TransferRepairItem(H4Input3, InputItem, InputItemIndex);
        TransferRepairItem(H4Input4, InputItem, InputItemIndex);
        TransferRepairItem(H4Input5, InputItem, InputItemIndex);
        TransferRepairItem(H4Input6, InputItem, InputItemIndex);

        InputItemIndex++;
    }
}

bool ARPDysonSphere::CanTransferItemForRepair(int AllowedItemIndex)
{
    int itemCount = mRepairItemCount[AllowedItemIndex];
    int maxAmount = mRepairMaxItemNum[AllowedItemIndex];
    if (itemCount < maxAmount)
    {
        return true;
    }

    return false;
}

void ARPDysonSphere::TransferRepairItem(UFGFactoryConnectionComponent* InputConnection,
                                        TSubclassOf<UFGItemDescriptor> InputItem, int InputItemIndex)
{
    FInventoryItem out_item;
    float out_Offsetbeyond;

    if (InputConnection->Factory_GrabOutput(out_item, out_Offsetbeyond, InputItem))
    {
        int max = mRepairMaxItemNum[InputItemIndex];
        int count = mRepairItemCount[InputItemIndex] + 1;

        mRepairItemCount[InputItemIndex] = FMath::Clamp(count, 0, max);
    }
}

void ARPDysonSphere::ClearRepairItemCount()
{
    int itemIndex = 0;

    for (int ItemCount : mRepairItemCount)
    {
        mRepairItemCount[itemIndex] = 0;
        itemIndex++;
    }
}

void ARPDysonSphere::Multicast_UpdateHangarLights_Implementation()
{
    OnRep_UpdateHangarLights();
}

void ARPDysonSphere::Multicast_StartShipAnimation_Implementation()
{
    OnRep_StartShipAnimation();
}

void ARPDysonSphere::Multicast_UpdateLightBeam_Implementation()
{
    OnRep_UpdateLightBeam();
}

void ARPDysonSphere::ResetFailedDysonSphere()
{
    auto rco = Cast<URPDysonSphereRCO>(
        Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(
            URPDysonSphereRCO::StaticClass()));

    if (rco)
    {
        rco->ResetFailedDysonSphere(this);
    }
}

int ARPDysonSphere::GetItemsRemainingForStage()
{
    int resCount = 0;

    if (mDysonSphereState == EDysonSphereState::RP_DS_State_Build)
    {
        int itemIndex = 0;
        for (int ItemCount : mBuildStageItemCount)
        {
            int count = mBuildStageItemCount[itemIndex];
            int max = mBuildStageMaxItemNum[itemIndex];

            resCount += (max - count);

            itemIndex++;
        }
    }
    else if (mDysonSphereState == EDysonSphereState::RP_DS_State_Producing)
    {
        int itemIndex = 0;
        for (int ItemCount : mRepairItemCount)
        {
            int count = mRepairItemCount[itemIndex];
            int max = mRepairMaxItemNum[itemIndex];

            resCount += (max - count);

            itemIndex++;
        }
    }

    return resCount;
}
