// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "ModularPower/RPMPBuilding.h"
#include "FGItemDescriptor.h"
#include "FGInventoryComponent.h"
#include "FGFactoryConnectionComponent.h"
#include "FGPipeConnectionComponent.h"
#include "RPMPHeater.h"
#include "util/Logging.h"
#include "RPMPBoiler.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPMPBoiler : public ARPMPBuilding
{
	GENERATED_BODY()
	
public:
	
	ARPMPBoiler();

	virtual void BeginPlay() override;
	virtual void Factory_Tick(float dt) override;
	virtual void Tick(float dt) override;

	// Functions See DRAW.IO heater tab for diagram!

	void CacheConnections();

	/* Collect & Store Items*/
	void CollectItems();

	/* BurnEnergy (Deincrement mCurrentHeatValue in connected heater every second) */

	/*Variables*/
private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		TSubclassOf<UFGItemDescriptor> mWaterItemClass;

	UFGPipeConnectionComponent* InputWaterPipe;
	UFGPipeConnectionComponent* OutputSteamPipe;

	/*the heater this boiler is connected to*/
	ARPMPHeater* connectedHeater;

	int mWaterInvIndex = 0;
	int mSteamInvIndex = 1;
};
