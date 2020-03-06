#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "GameFramework/Actor.h"
#include "FGBuildableGenerator.h"
#include "FGPowerConnectionComponent.h"
#include "Containers/Array.h"
#include "UnrealNetwork.h"
#include "RPTurbineBaseActor.generated.h"

UENUM(BlueprintType)
enum class ETurbineType : uint8
{
	RP_Wind 	UMETA(DisplayName = "Wind"),
	RP_Water 	UMETA(DisplayName = "Water")
};

UCLASS()
class REFINEDPOWER_API ARPTurbineBaseActor : public AFGBuildableGenerator {
	GENERATED_BODY()
public:

	ARPTurbineBaseActor();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	void calculateTurbinePowerProduction();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	float getTurbineBasePowerProduction();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	float getTurbineHeightPowerProduction();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	void setTurbinePowerOutput();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	TArray< ARPTurbineBaseActor*> getNearbyWindTurbineCount();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	void updateNearbyWindTurbineCount();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
		void updateTurbineEnabled(bool turbineEnabled);

	UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|Turbine")
	void updateTurbineParticleState();

protected:
	/** Amount of power this Turbine produces in MW. */
	UPROPERTY(VisibleDefaultsOnly, Category = "RefinedPower")
	float mTurbinePowerProduction;

	/** Is this turbine based on height eg higher height = more power */
	UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
	bool mIsHeightBasedTurbine;

	/** The type of Turbine, Wind Or Water */
	UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
	ETurbineType mTurbineType;

	/** Is turbine enabled or disabled */
	UPROPERTY(EditDefaultsOnly, SaveGame, Replicated, Category = "RefinedPower")
	bool mTurbineEnabled;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFGPowerConnectionComponent* FGPowerConnection;

	/** The amount of wind turbines in the area */
	UPROPERTY(VisibleAnywhere, Category = "RefinedPower")
	uint32 mWindTurbinesInArea;

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	void calcNearbyWindTurbines();

};