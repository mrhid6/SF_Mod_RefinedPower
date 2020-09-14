// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "ModularPower/RPMPBuilding.h"
#include "FGPipeConnectionComponent.h"
#include "RPMPTurbineBuilding.generated.h"

UCLASS()
class REFINEDPOWER_API URPMPTurbineBuildingRCO : public UFGRemoteCallObject
{
    GENERATED_BODY()

public:
    UFUNCTION(Server, WithValidation, Reliable)
    void SetSteamDiscard(ARPMPTurbineBuilding* panel, float value);

    UPROPERTY(Replicated)
    bool bTest = true;
};

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPMPTurbineBuilding : public ARPMPBuilding
{
    GENERATED_BODY()

public:
    ARPMPTurbineBuilding();

    virtual void BeginPlay() override;
    virtual void Factory_Tick(float dt) override;
    virtual void Tick(float dt) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // Functions
    void CacheConnections();
    void CollectSteam(float dt);

    void OutputSteam(float dt);

    void CalcTurbineState();

    bool CanStartSteamConsumption();
    void StartSteamConsumption();

    void ConvertSteamToRPM();

    void ReduceRPM();

    //RCO Functions
    UFUNCTION(BlueprintCallable, Category = "RefinedPower|ModularPower|Turbine")
    void SetSteamDiscard(float value);


    // Variables

    // RPM Stuff
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Replicated, Category = "RefinedPower")
    int mCurrentTurbineRPM = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    int mYellowMinTurbineRPM = 15000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    int mYellowMaxTurbineRPM = 19999;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    int mRedMinTurbineRPM = 20000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    int mRedMaxTurbineRPM = 25000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    int mRPMDrag = 20;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    int mRPMRate = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    float mRPMMultiplier = 2;

    // Steam Stuff

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    int mSteamPullAmount = 70;

    UPROPERTY(BlueprintReadOnly, SaveGame, Replicated, meta = (UIMin = "0", UIMax = "1.0"), Category =
        "RefinedPower")
    float mSteamDiscardPercent = 0.0f;

    // For BP Animations

    bool TriggerTurbineStateUpdated = false;

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_TurbineStateUpdated();

    UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|ModularPower")
    void OnRep_TurbineStateUpdated();

    // FIN Intergration
    UFUNCTION()
    int netFunc_getCurrentRPM();

    UFUNCTION()
    float netFunc_getSteamDiscardPercent();

    
    // Fluid Buffer

    virtual bool CanTransferToFluidBuffer() override;
    virtual void TransferToFluidBuffer() override;

    // Components

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    TSubclassOf<UFGItemDescriptor> mHighSteamItemClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    TSubclassOf<UFGItemDescriptor> mLowSteamItemClass;

    UFGPipeConnectionComponent* InputSteamPipe;
    UFGPipeConnectionComponent* OutputSteamPipe;

    int mInputInvIndex = 0;
    int mOutputInvIndex = 1;
};
