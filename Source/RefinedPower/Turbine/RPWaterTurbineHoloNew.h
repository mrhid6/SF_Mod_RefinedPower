// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Hologram/FGFactoryHologram.h"
#include "Components/BoxComponent.h"
#include "RPWaterTurbineNode.h"
#include "RPWaterTurbineHoloNew.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPWaterTurbineHoloNew : public AFGFactoryHologram
{
	GENERATED_BODY()

		ARPWaterTurbineHoloNew();
		~ARPWaterTurbineHoloNew();
	public:
		virtual void BeginPlay() override;
		virtual void SetHologramLocationAndRotation(const FHitResult& hitResult) override;
		virtual void CheckValidPlacement() override;

		bool CheckOverlapWaterVolume();

		bool CheckSnapLocations();
		bool IsPointInsideBox(FVector point, UBoxComponent* boxComponent);

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
			class UBoxComponent* mWaterTest;

		TArray<FVector> mSnapLocations;

		FVector mSnapToPoint = FVector(0);

		/*Array of all water volumes on map. Only call this once.*/
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
			TArray<AActor*> mCachedWaterNodeArr;

};
