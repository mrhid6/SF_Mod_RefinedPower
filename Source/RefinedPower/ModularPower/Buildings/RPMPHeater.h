// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "ModularPower/RPMPBuilding.h"
#include "FGItemDescriptor.h"
#include "FGInventoryComponent.h"
#include "FGFactoryConnectionComponent.h"
#include "FGPipeConnectionComponent.h"
#include "util/Logging.h"
#include "RPMPHeater.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPMPHeater : public ARPMPBuilding
{
	GENERATED_BODY()
	
public:

	ARPMPHeater();

	virtual void BeginPlay() override;
	virtual void Factory_Tick(float dt) override;
	virtual void Tick(float dt) override;

	// Functions See DRAW.IO heater tab for diagram!

	void CacheConnections();

	/* Collect & Store Items*/
	void CollectItems();

	/* CanStartItemBurn (checks amount of heat & co2 in inventory) */
	bool canBurnItem();

	/* StartItemBurn (Copy item energy value to mCurrentEnergyValue & mMaxEnergyValue) */
	void burnItem();

	/* HasItemBeenBurnt return if mCurrentEnergyValue = 0 */

	/* Produce heat from energy and store in counter */

	/* ProduceCo2 if needed and store in inventory */



	/*Util Functions*/

	bool canAddFuelItem(TSubclassOf<UFGItemDescriptor> itemClass);

	int getFuelItemCount();

	/**********************/

	/*Getters & setters*/

	/*returns the current heat of this heater*/
	int getCurrentHeatValue() { return this->mCurrentHeatValue; }

	/*adds to the current heat of this heater - used in the heater itself*/
	void addHeat(int heat) { this->mCurrentHeatValue += heat; }

	/*subtracts from the current heat of this heater - used in the connected boiler*/
	void subtractHeat(int heat) { this->mCurrentHeatValue -= heat; }

	/**********************/

	/*Variables*/

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		TArray<TSubclassOf<UFGItemDescriptor>> mAllowedFuelItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
		int mCurrentHeatValue = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
		int mMaxHeatValue = 1023; /*place holder value (needs balancing) - based on melting point of copper in Kelvin*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
		int mCurrentEnergyValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
		int mMaxEnergyValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		bool mProducesCo2 = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UFGInventoryComponent* mHeaterInventory;

	UFGFactoryConnectionComponent* InputFuelConveyor;
	UFGPipeConnectionComponent* InputFuelPipe;
	UFGPipeConnectionComponent* OutputCo2Pipe;


	int mFuelInvIndex = 0;
	int mCo2InvIndex = 1;

};
