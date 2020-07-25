// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Buildables/FGBuildableFactory.h"
#include "FGItemDescriptor.h"
#include "FGInventoryComponent.h"
#include "util/Logging.h"
#include "FGRemoteCallObject.h"
#include "UnrealNetwork.h"
#include "RPMPPlatform.h"
#include "FGItemDescriptor.h"
#include "FGInventoryComponent.h"
#include "FGFactoryConnectionComponent.h"
#include "FGPipeConnectionComponent.h"
#include "FGPowerInfoComponent.h"
#include "RPMPBuilding.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPMPBuilding : public AFGBuildableFactory
{
    GENERATED_BODY()
public:

    ARPMPBuilding();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void GetDismantleRefund_Implementation(TArray<FInventoryStack>& out_refund) const override;

    ARPMPPlatform* mAttachedPlatform;
    void GetAttachedPlatform(ARPMPPlatform* & Platform);

    void AttachToPlacementComp();
    void DetachFromPlacementComp();

    // Trigger Platform to update all Attached Buildings (BeginPlay/EndPlay)
    void TriggerUpdatePlatformAttachments();

    // Called when platform Attachment update is run ^^
    virtual void UpdateDependantBuildings()
    {
    };

    // Replicated Inventory Stuff

    UClass* GetReplicationDetailActorClass() const override;


    UFUNCTION(BlueprintPure, Category = "Inventory")
    FORCEINLINE class UFGInventoryComponent* GetMPInventory() const
    {
        return MPInventoryHandler->GetActiveInventoryComponent();
    }

    UPROPERTY(SaveGame)
    class UFGInventoryComponent* MPInventoryComponent;


    UFGReplicationDetailInventoryComponent* MPInventoryHandler;

    friend class AReplicationDetailActor_MPBuilding;
    void OnRep_ReplicationDetailActor() override;

    // Fluid Buffer

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RP Fluid Buffer")
    int mFluidBufferToLoad = 1000;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Replicated, Category = "RP Fluid Buffer")
    int mCurrentFluidBufferAmount = 0;

    /* Amount to take from buffer each tick */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RP Fluid Buffer")
    int mAmountFromBufferToTake = 5;

    virtual bool CanTransferToFluidBuffer();
    virtual void TransferToFluidBuffer();


    // Util Functions

    void StoreItemInInventory(UFGInventoryComponent* inventory, int InvIndex, TSubclassOf<UFGItemDescriptor> itemClass);
    void StoreItemInInventory(UFGInventoryComponent* inventory, int InvIndex, TSubclassOf<UFGItemDescriptor> itemClass,
                              int Amount);

    void StoreItemStackInInventory(UFGInventoryComponent* inventory, int InvIndex, FInventoryStack ItemStack);

    bool CanStoreItemInInventory(UFGInventoryComponent* inventory, int InvIndex,
                                 TSubclassOf<UFGItemDescriptor> itemClass, int amount);
};
