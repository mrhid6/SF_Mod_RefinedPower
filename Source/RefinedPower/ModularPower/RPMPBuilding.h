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
#include "RPMPBuilding.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPMPBuilding : public AFGBuildableFactory
{
	GENERATED_BODY()
public:

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    ARPMPPlatform* mAttachedPlatform;

    void GetAttachedPlatform(ARPMPPlatform* &Platform);

    void StoreItemInInventory(UFGInventoryComponent* inventory, int InvIndex, TSubclassOf<UFGItemDescriptor> itemClass);
    void StoreItemInInventory(UFGInventoryComponent* inventory, int InvIndex, TSubclassOf<UFGItemDescriptor> itemClass, int Amount);

    void StoreItemStackInInventory(UFGInventoryComponent* inventory, int InvIndex, FInventoryStack ItemStack);

    bool CanStoreItemInInventory(UFGInventoryComponent* inventory, int InvIndex, TSubclassOf<UFGItemDescriptor> itemClass, int amount);
};
