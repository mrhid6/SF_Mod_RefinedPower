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
    virtual void Factory_Tick(float dt) override;
    virtual void Tick(float dt) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void CacheTurbineBuilding();

    float GetRPMPowerCurveValue();

    void ConvertRPMToPower();

    ARPMPTurbineBuilding* mAttachedTurbine;

    virtual void UpdateDependantBuildings() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    float mMaxPowerOutput;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    int mMaxRPM;

    UPROPERTY(BlueprintReadOnly, Replicated, Category = "RefinedPower")
    int mCurrentRPM = 0;

    UPROPERTY(BlueprintReadOnly, Replicated, Category = "RefinedPower")
    float mCurrentPowerProduction = 0.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    UCurveFloat* mGeneratorCurve;
};
