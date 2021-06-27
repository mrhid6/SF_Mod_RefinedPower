#pragma once

#include "CoreMinimal.h"
#include "Buildables/FGBuildableGenerator.h"
#include "FGRemoteCallObject.h"
#include "RPDysonSphere.generated.h"

UENUM(BlueprintType)
enum class EDysonSphereState : uint8
{
    RP_DS_State_Build UMETA(DisplayName = "BuildStage"),
    RP_DS_State_LightBeam UMETA(DisplayName = "LightBeam"),
    RP_DS_State_Producing UMETA(DisplayName = "Producing"),
    RP_DS_State_Failed UMETA(DisplayName = "Failed")
};

UENUM(BlueprintType)
enum class EDSBuildState : uint8
{
    RP_DSB_State_0 UMETA(DisplayName = "Stage0"),
    RP_DSB_State_1 UMETA(DisplayName = "Stage1"),
    RP_DSB_State_2 UMETA(DisplayName = "Stage2"),
    RP_DSB_State_3 UMETA(DisplayName = "Stage3"),
    RP_DSB_State_Completed UMETA(DisplayName = "Completed")
};

UENUM(BlueprintType)
enum class EDSLightBeamState : uint8
{
    RP_DSLB_State_Grow UMETA(DisplayName = "Growing"),
    RP_DSLB_State_Completed UMETA(DisplayName = "Completed")
};

UCLASS()
class REFINEDPOWER_API ARPDysonSphere : public AFGBuildableGenerator
{
    GENERATED_BODY()

    ARPDysonSphere();
    ~ARPDysonSphere();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void BeginPlay() override;
    virtual void Factory_Tick(float dt) override;
    virtual void Tick(float dt) override;

public:

    void CalcUnlockedHangars();

    UFUNCTION(BlueprintPure, Category = "RefinedPower|DysonSphere")
    int GetUnlockedHangarCount();

    void CalcDysonSphereState(float dt);
    void SetDysonSphereState(EDysonSphereState state);
    bool IsDoorAnimationFinished();
    void StartDoorAnimationTimer();

    UFUNCTION(BlueprintPure, Category = "RefinedPower|DysonSphere")
    float GetPowerOutput();

    void SetPowerOutput();

    // Calculate Build State

    void CalcBuildState();
    void SetBuildStageState(EDSBuildState state);
    void CollectBuildItems();
    bool CanTransferItemForBuildState(int AllowedItemIndex);
    void TransferBuildStageItem(UFGFactoryConnectionComponent* InputConnection,
                                TSubclassOf<UFGItemDescriptor> InputItem, int InputItemIndex);

    bool IsBuildStageCompleted();
    void ClearBuildStageItemCount();

    // Calculate LightBeam State

    void CalcLightBeamState();
    void ResetLightBeam();
    void SetLightBeamFull();
    void GrowLightBeam();
    bool IsLightBeamCompleted();
    void SetLightBeamState(EDSLightBeamState state);

    // Calculate Producing State

    void CalcProducingState(float dt);
    void IncreaseRepairTimer(float dt);
    void ResetRepairTimer();
    bool IsRepairTimerFinished();

    UFUNCTION(BlueprintPure, Category = "RefinedPower|DysonSphere")
    float GetRepairTimerLimit();
    bool IsRepairItemsCompleted();
    void CollectRepairItems();
    bool CanTransferItemForRepair(int AllowedItemIndex);
    void TransferRepairItem(UFGFactoryConnectionComponent* InputConnection, TSubclassOf<UFGItemDescriptor> InputItem,
                            int InputItemIndex);
    void ClearRepairItemCount();

    UFUNCTION(NetMulticast, Unreliable)
    void Multicast_UpdateHangarLights();

    UFUNCTION(NetMulticast, Unreliable)
    void Multicast_StartShipAnimation();

    UFUNCTION(NetMulticast, Unreliable)
    void Multicast_UpdateLightBeam();

    UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|DysonSphere")
    void OnRep_UpdateHangarLights();

    UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|DysonSphere")
    void OnRep_StartShipAnimation();

    UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|DysonSphere")
    void OnRep_UpdateLightBeam();

    UFUNCTION(BlueprintCallable, Category = "RefinedPower|DysonSphere")
    void ResetFailedDysonSphere();

    UFUNCTION(BlueprintPure, Category = "RefinedPower|DysonSphere")
    int GetItemsRemainingForStage();

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateHangarLights, Category = "RefinedPower|DysonSphere")
    bool mHangar1Enabled = false;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateHangarLights, Category = "RefinedPower|DysonSphere")
    bool mHangar2Enabled = false;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateHangarLights, Category = "RefinedPower|DysonSphere")
    bool mHangar3Enabled = false;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_UpdateHangarLights, Category = "RefinedPower|DysonSphere")
    bool mHangar4Enabled = false;


    UPROPERTY(BlueprintReadOnly, SaveGame, Replicated, Meta = (NoAutoJson = true), Category = "RefinedPower|DysonSphere"
    )
    EDysonSphereState mDysonSphereState = EDysonSphereState::RP_DS_State_Build;

    UPROPERTY(BlueprintReadOnly, SaveGame, Replicated, Meta = (NoAutoJson = true))
    EDSBuildState mDSBuildState = EDSBuildState::RP_DSB_State_0;

    UPROPERTY(BlueprintReadOnly, SaveGame, Replicated, Meta = (NoAutoJson = true))
    EDSLightBeamState mDSLightBeamState = EDSLightBeamState::RP_DSLB_State_Grow;

    UPROPERTY(BlueprintReadOnly, SaveGame, Replicated, Category = "RefinedPower|DysonSphere")
    float mRepairTimer;

    UPROPERTY(BlueprintReadOnly, SaveGame, Replicated, Category = "RefinedPower|DysonSphere")
    int mFailedRepairs = 0;

    UPROPERTY(BlueprintReadOnly, Category = "RefinedPower|DysonSphere")
    int mMaxFailedRepairs = 10;


    bool mTriggerHangarLightEvent = false;
    bool mTriggerLightBeamEvent = false;
    bool mTriggerShipAnimEvent = false;

protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    TArray<TSubclassOf<class UFGItemDescriptor>> mAllowedInputItems;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    TArray<int> mBuildStageMaxItemNum;

    UPROPERTY(BlueprintReadOnly, SaveGame, Replicated, Category = "RefinedPower|DysonSphere")
    TArray<int> mBuildStageItemCount;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    TArray<int> mRepairMaxItemNum;

    UPROPERTY(BlueprintReadOnly, SaveGame, Replicated, Category = "RefinedPower|DysonSphere")
    TArray<int> mRepairItemCount;


    int mDoorAnimationTimer = 0;
    int mDoorAnimationTimerDuration = 2600;


    UPROPERTY(BlueprintReadOnly, SaveGame, ReplicatedUsing = OnRep_UpdateLightBeam)
    FVector mLightBeamScale = FVector(0.0f, 0.0f, 20.0f);

    UPROPERTY(BlueprintReadOnly, SaveGame, ReplicatedUsing = OnRep_UpdateLightBeam)
    bool mLightBeamVisiable = false;


    // Blueprint Components

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UFGPowerConnectionComponent* FGPowerConnection;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H1Input1;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H1Input2;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H1Input3;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H1Input4;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H1Input5;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H1Input6;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H2Input1;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H2Input2;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H2Input3;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H2Input4;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H2Input5;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H2Input6;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H3Input1;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H3Input2;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H3Input3;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H3Input4;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H3Input5;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H3Input6;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H4Input1;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H4Input2;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H4Input3;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H4Input4;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H4Input5;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
    UFGFactoryConnectionComponent* H4Input6;
};

UCLASS()
class REFINEDPOWER_API URPDysonSphereRCO : public UFGRemoteCallObject
{
    GENERATED_BODY()

    public:
    UFUNCTION(Server, WithValidation, Unreliable)
    void ResetFailedDysonSphere(ARPDysonSphere* reactor);


    UPROPERTY(Replicated)
    bool bTest = true;
};