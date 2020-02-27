#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "GameFramework/Actor.h"
#include "FGBuildableGenerator.h"
#include "FGPowerConnectionComponent.h"
#include "RPTurbineBaseActor.generated.h"



UCLASS()
class REFINEDPOWER_API ARPTurbineBaseActor : public AFGBuildableGenerator {
	GENERATED_BODY()
public:

	ARPTurbineBaseActor();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "RefinedPower")
	void calculateTurbinePowerProduction();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower")
	float getTurbinePowerProduction();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower")
	float getTurbineHeightPowerProduction();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower")
	void startTurbinePowerProduction();

protected:
	/** Amount of power this Turbine produces in MW. */
	UPROPERTY(VisibleDefaultsOnly, Category = "RefinedPower")
	float mTurbinePowerProduction;

	/** Is this turbine based on height eg higher height = more power */
	UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
	bool mIsHeightBasedTurbine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFGPowerConnectionComponent* FGPowerConnection;

};