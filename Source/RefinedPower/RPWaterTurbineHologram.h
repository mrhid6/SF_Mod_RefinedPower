// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Hologram/FGFactoryHologram.h"
#include "Components/BoxComponent.h"
#include "FGWaterVolume.h"
#include "RPWaterTurbineHologram.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPWaterTurbineHologram : public AFGFactoryHologram
{
	GENERATED_BODY()

		ARPWaterTurbineHologram();
public:
	virtual void BeginPlay() override;
	virtual void Tick(float dt) override;
	virtual bool IsValidHitResult(const FHitResult& hitResult) const override;
	virtual void SetHologramLocationAndRotation(const FHitResult& hitResult) override;
	virtual void CheckValidPlacement() override;

	bool CheckOverlapWaterVolume(AFGWaterVolume* &foundWater) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* mWaterTest;

protected:

	AFGWaterVolume* mFoundWater;

};
