#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "FactoryGame/Public/Buildables/FGBuildableGenerator.h"
#include "FGPowerConnectionComponent.h"
#include "Containers/Array.h"
#include "Engine.h"
#include "FGRemoteCallObject.h"
#include "RPTurbineBaseActor.generated.h"

UENUM(BlueprintType)
enum class ETurbineType : uint8
{
    RP_Wind UMETA(DisplayName = "Wind"),
    RP_Water UMETA(DisplayName = "Water")
};

UCLASS()
class REFINEDPOWER_API ARPTurbineBaseActor : public AFGBuildableGenerator
{
    GENERATED_BODY()
public:

    ARPTurbineBaseActor();
    ~ARPTurbineBaseActor();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
    virtual bool ShouldSave_Implementation() const override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void LineTraceToGround();
    
    virtual void calculateTurbinePowerProduction();
    float getTurbineBasePowerProduction();
    float getTurbineHeightPowerProduction();
    void setTurbinePowerOutput();

    UFUNCTION(BlueprintPure, Category = "RefinedPower|Turbine")
    float getTurbineActualPowerProduction();

    UFUNCTION(BlueprintPure, Category = "RefinedPower|Turbine")
    float getMaxTurbinePowerProduction();

    UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
    void UpdateCachedNearbyWindTurbines();

    UFUNCTION(BlueprintPure, Category = "RefinedPower|Turbine")
    int getNearbyWindTurbinesCount();

    void updateNearbyWindTurbineCount();

    UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
    void setTurbineEnabled(bool turbineEnabled);

    UFUNCTION(BlueprintPure, Category = "RefinedPower|Turbine")
    bool isTurbineEnabled();

    UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|Turbine")
    void TurbineStateUpdated();


    /** Is turbine enabled or disabled */
    UPROPERTY(SaveGame, ReplicatedUsing = TurbineStateUpdated, meta = (NoAutoJson = true))
    bool mTurbineEnabled;

    TArray<ARPTurbineBaseActor*> mCachedNearbyWindTurbines;

protected:

    float mTurbinePowerProduction;

    /** Max power this turbine can produce */
    UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
    float mMaxTurbinePowerProduction;

    /** Is this turbine based on height eg higher height = more power */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    bool mIsHeightBasedTurbine;

    /** The type of Turbine, Wind Or Water */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    ETurbineType mTurbineType;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UFGPowerConnectionComponent* FGPowerConnection;

    UPROPERTY(Replicated)
    int mWindTurbinesInArea;

    /** The max amount of other wind turbines in the area */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    int mMaxWindTurbinesInArea;

    /** The max amount of other wind turbines in the area */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    float mAreaOfWindTurbines = 5600;

    UPROPERTY(BlueprintReadOnly, Replicated, Category = "RefinedPower")
    float mDistanceFromRound = 0.0f;

    void calcNearbyWindTurbines();
};

UCLASS()
class REFINEDPOWER_API URPTurbineBaseRCO : public UFGRemoteCallObject
{
    GENERATED_BODY()

    public:
    UFUNCTION(Server, WithValidation, Unreliable)
    void SetTurbineEnabled(ARPTurbineBaseActor* turbine, bool enabled);

    UPROPERTY(Replicated)
    bool bTest = true;
};