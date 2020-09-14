// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "ModularPower/RPMPBuilding.h"
#include "RPMPHeaterBuilding.h"
#include "Curves/CurveFloat.h"
#include "RPMPBoilerBuilding.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPMPBoilerBuilding : public ARPMPBuilding
{
    GENERATED_BODY()

public:

    ARPMPBoilerBuilding();

    virtual void BeginPlay() override;
    virtual void Factory_Tick(float dt) override;
    virtual void Tick(float dt) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void UpdateDependantBuildings() override;

    // Functions See DRAW.IO heater tab for diagram!

    void CacheConnections();

    /* Collect & Store Items*/
    void CollectItems(float dt);

    void OutputSteam(float dt);

    void CacheHeaterBuilding();

    bool CanGenerateSteam();
    void GenerateSteam();
    /* BurnEnergy (Deincrement mCurrentHeatValue in connected heater every second) */

    // Util Functions

    int getWaterItemCount();
    int getSteamItemCount();

    float GetSteamProductionCurveValue();

    // Fluid Buffer

    virtual bool CanTransferToFluidBuffer() override;
    virtual void TransferToFluidBuffer() override;

    /*Variables*/

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    TSubclassOf<UFGItemDescriptor> mWaterItemClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    TSubclassOf<UFGItemDescriptor> mSteamItemClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    int mWaterPullAmount = 100;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    float mSteamProductionMultiplier = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    UCurveFloat* mBoilerCurve;

    UFGPipeConnectionComponent* InputWaterPipe;
    UFGPipeConnectionComponent* OutputSteamPipe;

    /*the heater this boiler is connected to*/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    ARPMPHeaterBuilding* mAttachedHeater;

    int mWaterInvIndex = 0;
    int mSteamInvIndex = 1;

    int TickCounter;
};
