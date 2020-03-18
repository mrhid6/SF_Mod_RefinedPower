// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Hologram/FGFactoryHologram.h"
#include "RPWaterTurbineHologram.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPWaterTurbineHologram : public AFGFactoryHologram
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual bool IsValidHitResult(const FHitResult& hitResult) const override;
	//void CheckValidPlacement() override;
	void testScript();
};
