// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Buildables/FGBuildableFactory.h"
#include "FGItemDescriptor.h"
#include "FGInventoryComponent.h"
#include "RPMPBuilding.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPMPBuilding : public AFGBuildableFactory
{
	GENERATED_BODY()
public:

    void StoreItemInInventory(UFGInventoryComponent* inventory, int InvIndex, TSubclassOf<UFGItemDescriptor> itemClass);

    bool CanStoreItemInInventory(UFGInventoryComponent* inventory, int InvIndex, TSubclassOf<UFGItemDescriptor> itemClass);
};
