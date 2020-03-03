#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "GameFramework/Actor.h"
#include "FGBuildableGenerator.h"
#include "FGPowerConnectionComponent.h"
#include "Containers/Array.h"
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	void calculateTurbinePowerProduction();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	float getTurbinePowerProduction();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	float getTurbineHeightPowerProduction();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	void startTurbinePowerProduction();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	TArray< ARPTurbineBaseActor*> getNearbyTurbineCount();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	void updateNearbyTurbineCount();

	

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFGPowerConnectionComponent* FGPowerConnection;

	UPROPERTY(VisibleAnywhere, Category = "RefinedPower")
	uint32 mTurbinesInArea;

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	void calcNearbyTurbines();

};