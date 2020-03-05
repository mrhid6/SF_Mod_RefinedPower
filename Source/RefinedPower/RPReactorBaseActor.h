#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "GameFramework/Actor.h"
#include "FGBuildableGeneratorFuel.h"
#include "FGPowerConnectionComponent.h"
#include "RPReactorBaseActor.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPReactorBaseActor : public AFGBuildableGenerator
{
	GENERATED_BODY()
	public:

		ARPReactorBaseActor();

		UFUNCTION(BlueprintCallable, Category = "RefinedPower")
			float getBaseReactorPowerProduction();

		UFUNCTION(BlueprintCallable, Category = "RefinedPower")
			void startReactorPowerProduction();

		UFUNCTION(BlueprintCallable, Category = "RefinedPower")
			void setBaseReactorPowerProduction(float p);

		UFUNCTION(BlueprintCallable, Category = "RefinedPower")
			int collectInputResource(UFGFactoryConnectionComponent* inputConnection, TSubclassOf<UFGItemDescriptor> type, const int maxStorage, int& inputAmount);

	protected:

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
			class UFGPowerConnectionComponent* FGPowerConnection;
};
