// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "ModularPower/RPMPBuilding.h"
#include "FGItemDescriptor.h"
#include "FGInventoryComponent.h"
#include "FGFactoryConnectionComponent.h"
#include "FGPipeConnectionComponent.h"
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

	// Functions See DRAW.IO Boiler tab for diagram!

	void CacheConnections();

	/* Collect & Store Items*/
	void CollectItems();

	/* CanStartItemBurn (checks amount of steam&co2 in inventory) */

	/* StartItemBurn (Copy item energy value to mCurrentEnergyValue & mMaxEnergyValue) */

	/* BurnEnergy (Deincrement mCurrentEnergyValue every second) */

	/* HasItemBeenBurnt return if mCurrentEnergyValue = 0 */

	/* ProduceSteam and store in inventory */

	/* ProduceCo2 if needed and store in inventory */



	// Util Functions

	bool canAddFuelItem(TSubclassOf<UFGItemDescriptor> itemClass);

	int getFuelItemCount();

	// Variables

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
	TArray<TSubclassOf<UFGItemDescriptor>> mAllowedFuelItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		TArray<TSubclassOf<UFGItemDescriptor>> mAllowedWaterItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
	int mCurrentEnergyValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
	int mMaxEnergyValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
	bool mProducesCo2 = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UFGInventoryComponent* mBoilerInventory;

		UFGFactoryConnectionComponent* InputFuelConveyor;
		UFGPipeConnectionComponent* InputFuelPipe;
		UFGPipeConnectionComponent* InputWaterPipe;
		UFGPipeConnectionComponent* OutputSteamPipe;
		UFGPipeConnectionComponent* OutputCo2Pipe;


	int mFuelInvIndex = 0;
	int mWaterInvIndex = 1;
	int mSteamInvIndex = 2;
	int mCo2InvIndex = 3;

};
