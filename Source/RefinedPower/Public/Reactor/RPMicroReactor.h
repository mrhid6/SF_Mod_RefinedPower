#pragma once

#include "CoreMinimal.h"
#include "Core.h"
#include "Buildables/FGBuildableGeneratorFuel.h"
#include "FGRemoteCallObject.h"
#include "RPMicroReactor.generated.h"

UENUM(BlueprintType)
enum class EMicroReactorState : uint8
{
    RP_State_Cold UMETA(DisplayName = "Cold"),
    RP_State_WarmUp UMETA(DisplayName = "WarmUp"),
    RP_State_Producing UMETA(DisplayName = "Producing")
};

UCLASS()
class REFINEDPOWER_API URPMicroReactorRCO : public UFGRemoteCallObject
{
    GENERATED_BODY()

public:

    UPROPERTY(Replicated)
    bool bTest = true;
};

UCLASS()
class REFINEDPOWER_API ARPMicroReactor : public AFGBuildableGeneratorFuel
{
    GENERATED_BODY()

    ARPMicroReactor();
    ~ARPMicroReactor();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void BeginPlay() override;
    virtual void Factory_Tick(float dt) override;
    virtual void Tick(float dt) override;

    virtual bool CanStartPowerProduction_Implementation() const override;

public:

    void CalcReactorState();

    UFUNCTION(BlueprintPure)
    int getReactorCores();

    UFUNCTION(BlueprintPure)
    int getReactorCoresMax();

    void CacheFuelAmount();

    void SetReactorState(EMicroReactorState state);

    UFUNCTION(BlueprintPure)
    EMicroReactorState GetReactorState();

    UFUNCTION(BlueprintPure)
    int GetReactorMinStart();

    UFUNCTION(BlueprintPure)
    int GetReactorMinStop();

    UFUNCTION(BlueprintPure)
    float GetWarmUpAmount();

    void IncreaseWarmUp();

    void ResetWarmUp();

    UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|Reactor")
    void ReactorStateUpdated();


protected:

    UPROPERTY(ReplicatedUsing = ReactorStateUpdated, SaveGame)
    EMicroReactorState mReactorState = EMicroReactorState::RP_State_Cold;

    UPROPERTY(Replicated, SaveGame)
    float mWarmUpAmount;

    UPROPERTY(Replicated)
    int mCachedReactorCoresAmount;

    UPROPERTY(Replicated)
    int mCachedReactorCoresMax;

    /*Min amount of resources required to start production [0-1]*/
    UPROPERTY(EditDefaultsOnly, Category = "RefinedPower", meta=(ClampMin = "0.0", ClampMax = "1.0"))
    float mMinStartAmount = 0.75f;

    /*When the resources fall below this amount, the reactor will shut off [0-1]*/
    UPROPERTY(EditDefaultsOnly, Category = "RefinedPower", meta=(ClampMin = "0.0", ClampMax = "1.0"))
    float mMinStopAmount = 0.4f;

    bool mTriggerReactorStateUpdatedEvent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* InputConveyor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UFGPowerConnectionComponent* FGPowerConnection;
};
