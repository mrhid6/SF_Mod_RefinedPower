// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "RefinedPower/Public/ModularPower/Buildings/RPMPHeaterBuilding.h"
#include "RPMPSolutionHeaterBuilding.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPMPSolutionHeaterBuilding : public ARPMPHeaterBuilding
{
	GENERATED_BODY()

	ARPMPSolutionHeaterBuilding();
	
	virtual void BeginPlay() override;
	virtual void Factory_Tick(float dt) override;
	
	/* CanStartItemBurn (checks amount of heat & co2 in inventory) */
	virtual bool CanStartItemBurn() override;

	/* StartItemBurn (Copy item energy value to mCurrentEnergyValue & mMaxEnergyValue) */
	virtual void BurnItem(float dt) override;
	

	// Fluid Buffer

	virtual bool CanTransferToFluidBuffer() override;
	virtual void TransferToFluidBuffer() override;

	
};
