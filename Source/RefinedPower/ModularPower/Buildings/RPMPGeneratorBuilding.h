// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "ModularPower/RPMPBuilding.h"
#include "RPMPTurbineBuilding.h"
#include "RPMPGeneratorBuilding.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPMPGeneratorBuilding : public ARPMPBuilding
{
	GENERATED_BODY()
public:
	ARPMPGeneratorBuilding();

	virtual void BeginPlay() override;

	void CacheTurbineBuilding();

	ARPMPTurbineBuilding* mAttachedTurbine;

	virtual void UpdateDependantBuildings() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		float mMaxPowerOutput;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		int mMaxRPM;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		UCurveFloat* mGeneratorCurve;

};
