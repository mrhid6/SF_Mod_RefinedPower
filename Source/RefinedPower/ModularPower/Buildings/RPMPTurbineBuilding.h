// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "ModularPower/RPMPBuilding.h"
#include "RPMPTurbineBuilding.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPMPTurbineBuilding : public ARPMPBuilding
{
	GENERATED_BODY()

public:
	ARPMPTurbineBuilding();

	virtual void BeginPlay() override;
	virtual void Factory_Tick(float dt) override;
	virtual void Tick(float dt) override;

	// Variables

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UFGInventoryComponent* mTurbineInventory;
	
};
