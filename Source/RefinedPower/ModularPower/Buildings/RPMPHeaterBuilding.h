// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "ModularPower/RPMPBuilding.h"
#include "FGItemDescriptor.h"
#include "FGInventoryComponent.h"
#include "FGFactoryConnectionComponent.h"
#include "FGPipeConnectionComponent.h"
#include "RPMPHeaterBuilding.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPMPHeaterBuilding : public ARPMPBuilding
{
    GENERATED_BODY()

public:

    ARPMPHeaterBuilding();

    virtual void BeginPlay() override;
    virtual void Factory_Tick(float dt) override;
    virtual void Tick(float dt) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // Functions See DRAW.IO heater tab for diagram!

    void CacheConnections();

    /* Collect & Store Items*/
    void CollectItems(float dt);

    /* CanStartItemBurn (checks amount of heat & co2 in inventory) */
    virtual bool CanStartItemBurn();

    /* StartItemBurn (Copy item energy value to mCurrentEnergyValue & mMaxEnergyValue) */
    virtual void BurnItem(float dt);

    /* ProduceCo2 if needed and store in inventory */
    void ProduceCo2();
    
    void ProduceNukeWaste();

    void OutputCo2(float dt);


    /*Util Functions*/

    int getFuelItemCount();

    int getCo2ItemCount();

    /**********************/

    /*Getters & setters*/

    /*returns the current heat of this heater*/
    int getCurrentHeatValue() { return this->mCurrentHeatValue; }

    /*adds to the current heat of this heater - used in the heater itself*/
    void addHeat(float heat)
    {
        this->mCurrentHeatValue += heat;
        mCurrentHeatValue = FMath::Clamp(mCurrentHeatValue, 0.0f, mMaxHeatValue);
    }

    /*subtracts from the current heat of this heater - used in the connected boiler*/
    void subtractHeat(float heat)
    {
        mCurrentHeatValue -= heat;
        mCurrentHeatValue = FMath::Clamp(mCurrentHeatValue, 0.0f, mMaxHeatValue);
    }

    UFUNCTION()
    bool FilterFuelClasses( TSubclassOf< UObject > object, int32 idx ) const;

    virtual bool CheckMPBuildingRunningState() override;

    // Co2ProductionRate

    void UpdateCo2ProductionRate();

    /**********************/

    // Variables

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    TArray<TSubclassOf<UFGItemDescriptor>> mAllowedFuelItems;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    TSubclassOf<UFGItemDescriptor> mCo2ItemClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    TSubclassOf<UFGItemDescriptor> mNukeWasteItemClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Replicated, Category = "RefinedPower")
    float mCurrentHeatValue = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    float mMaxHeatValue = 500; /*place holder value (needs balancing) - based on melting point of copper in Kelvin*/

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Replicated, Category = "RefinedPower")
    float mCurrentEnergyValue;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Replicated, Category = "RefinedPower")
    float mMaxEnergyValue;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame, Replicated, Category = "RefinedPower")
    float mEnergyValueMultiplier = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    bool mProducesCo2 = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    bool mProducesNukeWaste = false;

    /* Amount of Co2 to generate per tick */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    int mOutputGenerationAmount = 5;


    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    bool mIsFluidHeater = false;
    
    UFGFactoryConnectionComponent* InputFuelConveyor;
    UFGPipeConnectionComponent* InputFuelPipe;
    UFGPipeConnectionComponent* OutputCo2Pipe;
    UFGFactoryConnectionComponent* OutputWasteConveyor;


    int mInputInvIndex = 0;
    int mOutputInvIndex = 1;
};
