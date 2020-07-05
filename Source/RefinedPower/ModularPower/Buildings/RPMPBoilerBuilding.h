// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "ModularPower/RPMPBuilding.h"
#include "FGItemDescriptor.h"
#include "FGInventoryComponent.h"
#include "FGFactoryConnectionComponent.h"
#include "FGPipeConnectionComponent.h"
#include "RPMPHeaterBuilding.h"
#include "util/Logging.h"
#include "RPMPBoilerBuilding.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPMPBoilerBuilding : public ARPMPBuilding
{
	GENERATED_BODY()
	
public:
	
	ARPMPBoilerBuilding();

	virtual void BeginPlay() override;
	virtual void Factory_Tick(float dt) override;
	virtual void Tick(float dt) override;

	virtual void UpdateDependantBuildings() override;

	// Functions See DRAW.IO heater tab for diagram!

	void CacheConnections();

	/* Collect & Store Items*/
	void CollectItems();

	void CacheHeaterBuilding();

	/* BurnEnergy (Deincrement mCurrentHeatValue in connected heater every second) */

	/*Variables*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		TSubclassOf<UFGItemDescriptor> mWaterItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		TSubclassOf<UFGItemDescriptor> mSteamItemClass;

	UFGPipeConnectionComponent* InputWaterPipe;
	UFGPipeConnectionComponent* OutputSteamPipe;

	/*the heater this boiler is connected to*/
	ARPMPHeaterBuilding* mAttachedHeater;

	int mWaterInvIndex = 0;
	int mSteamInvIndex = 1;
};
