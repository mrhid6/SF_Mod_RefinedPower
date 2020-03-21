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
		~ARPWaterTurbineHologram();
	public:
		virtual void BeginPlay() override;
		virtual void SetHologramLocationAndRotation(const FHitResult& hitResult) override;
		virtual void CheckValidPlacement() override;

		bool CheckOverlapWaterVolume();

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
			class UBoxComponent* mWaterTest;

		/*Array of all water volumes on map. Only call this once.*/
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
			TArray<AActor*> mCachedWaterVolArr;

	protected:

		AFGWaterVolume* mFoundWater;

};
