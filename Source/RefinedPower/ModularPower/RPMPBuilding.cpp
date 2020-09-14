// ILikeBanas


#include "RPMPBuilding.h"
#include "FGInventoryLibrary.h"
#include "Buildings/ReplicationDetailActor_MPBuilding.h"



void URPMPBuildingRCO::SetSoundEnabled_Implementation(ARPMPBuilding* Building, bool enabled)
{
    Building->mIsSoundEnabled = enabled;
    Building->OnSoundEnabledChanged();
    Building->ForceNetUpdate();
}

bool URPMPBuildingRCO::SetSoundEnabled_Validate(ARPMPBuilding* Building, bool enabled)
{
    return true;
}

void URPMPBuildingRCO::SetParticlesEnabled_Implementation(ARPMPBuilding* Building, bool enabled)
{
    Building->mIsParticlesEnabled = enabled;
    Building->OnParticlesEnabledChanged();
    Building->ForceNetUpdate();
}

bool URPMPBuildingRCO::SetParticlesEnabled_Validate(ARPMPBuilding* Building, bool enabled)
{
    return true;
}

void URPMPBuildingRCO::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(URPMPBuildingRCO, bTest);
}


ARPMPBuilding::ARPMPBuilding()
{
    MPInventoryHandler = CreateDefaultSubobject<UFGReplicationDetailInventoryComponent>(
        TEXT("ReplicationDetailComponent_MPInventory"));
   
    mFINPowerConnection = CreateDefaultSubobject<UFGPowerConnectionComponent>(TEXT("FINPowerConnection"));
    mFINPowerConnection->SetupAttachment(RootComponent);
    
    //Initialize inventories, set default inventory sizes
            MPInventoryComponent = UFGInventoryLibrary::CreateInventoryComponent(this, TEXT("MPInventory"));
            MPInventoryComponent->SetDefaultSize(2);

    SetReplicates(true);
}

void ARPMPBuilding::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        GetAttachedPlatform(mAttachedPlatform);

        AttachToPlacementComp();

        TriggerUpdatePlatformAttachments();

        //Set main inventory reference for replication inventory handler
        //When ReplicationDetailActor is NOT SPAWNED, these represent REAL inventory states
        //But when one appears, it will copy state from then and from that moment we ARE NOT the owner of the inventories
        MPInventoryHandler->SetMainInventoryComponent(MPInventoryComponent);

        CalculateUsageRates();
        
        FTimerHandle timerHandle;
        GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ARPMPBuilding::CalculateUsageRates,
                                           60.0f, true);
    }
}

void ARPMPBuilding::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    if (endPlayReason == EEndPlayReason::Destroyed)
    {
        TriggerUpdatePlatformAttachments();
        DetachFromPlacementComp();
    }
    Super::EndPlay(endPlayReason);
}

void ARPMPBuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARPMPBuilding, mCurrentFluidBufferAmount)
    DOREPLIFETIME(ARPMPBuilding, mIsMPBuildingRunning)
    DOREPLIFETIME(ARPMPBuilding, mConsumptionRate)
    DOREPLIFETIME(ARPMPBuilding, mProductionRate)
}

void ARPMPBuilding::Tick(float dt)
{
    Super::Tick(dt);

    if(HasAuthority())
    {
        bool oldRunning = mIsMPBuildingRunning;
        mIsMPBuildingRunning = CheckMPBuildingRunningState();

        if(oldRunning != mIsMPBuildingRunning)
        {
            Multicast_RunningStateChanged();
        }
    }
}


UClass* ARPMPBuilding::GetReplicationDetailActorClass() const
{
    return AReplicationDetailActor_MPBuilding::StaticClass();
}

void ARPMPBuilding::OnRep_ReplicationDetailActor()
{
    Super::OnRep_ReplicationDetailActor();
    //SML::Logging::info("[RefinedPower] - Starting Inv Replication!");
    
    if (!HasAuthority() && mReplicationDetailActor)
    {
        //Cast replication detail actor, set active replication inventory component
        //On client these will be used for UI and other logic run on client side
        AReplicationDetailActor_MPBuilding* DetailActor = Cast<AReplicationDetailActor_MPBuilding>(
            mReplicationDetailActor);
        if (DetailActor->HasCompletedInitialReplication())
        {
            //SML::Logging::info("[RefinedPower] - Finished Inv Replication!");
            MPInventoryHandler->SetReplicationInventoryComponent(DetailActor->MPInventoryComponent);
        }
        else
        {
            //SML::Logging::info("[RefinedPower] - Waiting For Inv Replication!");
            FTimerManager& TimerManager = this->GetWorldTimerManager();
            FTimerHandle TimerHandle;
            TimerManager.SetTimer(TimerHandle, this, &ARPMPBuilding::OnRep_ReplicationDetailActor, 0.5f, false);
        }
    }else
    {
        //SML::Logging::info("[RefinedPower] - HasAuthority:");
        //SML::Logging::info(HasAuthority());
        //SML::Logging::info("[RefinedPower] - RepActor Null:");
        //SML::Logging::info(mReplicationDetailActor == nullptr);
    }
}

void ARPMPBuilding::TriggerUpdatePlatformAttachments()
{
    ARPMPPlatform* platform;
    GetAttachedPlatform(platform);

    if (platform)
    {
        platform->UpdatePlatformAttachments();
    }
}

void ARPMPBuilding::GetDismantleRefund_Implementation(TArray<FInventoryStack>& out_refund) const
{
    GetDismantleRefundReturns(out_refund);
    GetDismantleBlueprintReturns(out_refund);

    TArray<FInventoryStack> stacks;
    GetMPInventory()->GetInventoryStacks(stacks);

    for (FInventoryStack stack : stacks)
    {
        if (stack.HasItems())
        {
            EResourceForm form = UFGItemDescriptor::GetForm(stack.Item.ItemClass);

            if (form == EResourceForm::RF_SOLID)
            {
                out_refund.Add(stack);
            }
        }
    }
}

void ARPMPBuilding::GetAttachedPlatform(ARPMPPlatform* & Platform)
{
    const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = TArray<TEnumAsByte<EObjectTypeQuery>>{
        EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery2
    };
    TArray<AActor*> ActorsToIgnore = TArray<AActor*>{this};
    TArray<AActor*> OutActors;


    UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), 50, ObjectTypes, ARPMPPlatform::StaticClass(),
                                              ActorsToIgnore, OutActors);
    if (OutActors.Num() > 0)
    {
        Platform = Cast<ARPMPPlatform>(OutActors[0]);
    }
    else
    {
        Platform = nullptr;
    }

    if (Platform == nullptr)
    {
        // If we cant find it do a line trace down 1000 units (Used in Heater/Boiler)
        // TODO Optimise Units to mesh sizes

        FVector ForwardVector = FVector(0, 0, -1);
        FVector CompLocation = GetActorLocation();

        FVector Start = (CompLocation);

        FVector End = ((ForwardVector * 1000) + Start);
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(this);
        FHitResult OutHit;
        GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

        if (OutHit.Actor != nullptr)
        {
            if (OutHit.Actor->IsA(ARPMPPlatform::StaticClass()))
            {
                Platform = Cast<ARPMPPlatform>(OutHit.Actor);
            }
            else if (OutHit.Actor->IsA(ARPMPBuilding::StaticClass()))
            {
                ARPMPBuilding* building = Cast<ARPMPBuilding>(OutHit.Actor);

                building->GetAttachedPlatform(Platform);
            }
        }
    }
}

void ARPMPBuilding::AttachToPlacementComp()
{
    if (mAttachedPlatform)
    {
        mAttachedPlatform->AttachBuildingToComp(this);
    }
}

void ARPMPBuilding::DetachFromPlacementComp()
{
    if (mAttachedPlatform)
    {
        mAttachedPlatform->DetachBuildingFromComp(this);
    }
}


bool ARPMPBuilding::CanTransferToFluidBuffer()
{
    return false;
}

void ARPMPBuilding::TransferToFluidBuffer()
{
}


void ARPMPBuilding::StoreItemInInventory(UFGInventoryComponent* inventory, int InvIndex,
                                         TSubclassOf<UFGItemDescriptor> itemClass)
{
    StoreItemInInventory(inventory, InvIndex, itemClass, 1);
}

void ARPMPBuilding::StoreItemInInventory(UFGInventoryComponent* inventory, int InvIndex,
                                         TSubclassOf<UFGItemDescriptor> itemClass, int amount)
{
    if (CanStoreItemInInventory(inventory, InvIndex, itemClass, amount))
    {
        FInventoryItem outItem;
        outItem.ItemClass = itemClass;

        FInventoryStack itemStack;
        itemStack.NumItems = amount;
        itemStack.Item = outItem;
        inventory->AddStackToIndex(InvIndex, itemStack, true);
    }
    else
    {
        FInventoryStack out_stack;
        inventory->GetStackFromIndex(InvIndex, out_stack);
        int stackSize = UFGItemDescriptor::GetStackSize(out_stack.Item.ItemClass);

        int quantity = stackSize - out_stack.NumItems;

        if (CanStoreItemInInventory(inventory, InvIndex, itemClass, quantity))
        {
            StoreItemInInventory(inventory, InvIndex, itemClass, quantity);
        }
    }
}

void ARPMPBuilding::StoreItemStackInInventory(UFGInventoryComponent* inventory, int InvIndex, FInventoryStack ItemStack)
{
    if (ItemStack.HasItems() == false)
    {
        return;
    }

    StoreItemInInventory(inventory, InvIndex, ItemStack.Item.ItemClass, ItemStack.NumItems);
}

bool ARPMPBuilding::CanStoreItemInInventory(UFGInventoryComponent* inventory, int InvIndex,
                                            TSubclassOf<UFGItemDescriptor> itemClass, int amount)
{
    FInventoryStack out_stack;
    inventory->GetStackFromIndex(InvIndex, out_stack);
    if (out_stack.HasItems())
    {
        if (itemClass != out_stack.Item.ItemClass)
        {
            return false;
        }

        int stackSize = UFGItemDescriptor::GetStackSize(out_stack.Item.ItemClass);

        if (out_stack.NumItems >= stackSize || (out_stack.NumItems + amount) > stackSize)
        {
            return false;
        }
    }

    return true;
}

void ARPMPBuilding::Multicast_RunningStateChanged_Implementation()
{
    OnRunningStateChanged();
}

bool ARPMPBuilding::CheckMPBuildingRunningState()
{
    return false;
}

void ARPMPBuilding::UI_SetSoundEnabled(bool enabled)
{
    auto rco = Cast<URPMPBuildingRCO>(
        Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(
            URPMPBuildingRCO::StaticClass()));

    if (rco)
    {
        rco->SetSoundEnabled(this, enabled);
    }
}

void ARPMPBuilding::UI_SetParticlesEnabled(bool enabled)
{
    auto rco = Cast<URPMPBuildingRCO>(
    Cast<AFGPlayerController>(GetWorld()->GetFirstPlayerController())->GetRemoteCallObjectOfClass(
        URPMPBuildingRCO::StaticClass()));

    if (rco)
    {
        rco->SetParticlesEnabled(this, enabled);
    }
}

void ARPMPBuilding::CalculateUsageRates()
{
    mConsumptionRate = (mConsumptionTotal  / 60.0f) / 1000.0f;
    mProductionRate = (mProductionTotal  / 60.0f) / 1000.0f;


    /*SML::Logging::info("#####################");
    SML::Logging::info("Consumption:");
    SML::Logging::info(mConsumptionTotal);
    SML::Logging::info("Production:");
    SML::Logging::info(mProductionTotal);
    SML::Logging::info("#####################");*/

    mConsumptionTotal = 0.0f;
    mProductionTotal = 0.0f;
    

}