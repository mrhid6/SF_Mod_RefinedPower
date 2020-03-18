// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Hologram/FGBuildableHologram.h"
#include "Components/BoxComponent.h"
#include "FGWaterVolume.h"
#include "RPWaterTurbineHologram.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPWaterTurbineHologram : public AFGBuildableHologram
{
	GENERATED_BODY()

		ARPWaterTurbineHologram();
public:
	virtual void BeginPlay() override;
	virtual bool IsValidHitResult(const FHitResult& hitResult) const override;
	virtual void SetHologramLocationAndRotation(const FHitResult& hitResult) override;
	//void CheckValidPlacement() override;
	void testScript();

	bool SetOverlapWaterVolume() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* WaterTest;

protected:

	AFGWaterVolume* FoundWater;

};
