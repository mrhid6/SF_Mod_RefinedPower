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

		//virtual void BeginPlay() override;
		/*
		UFUNCTION(BlueprintCallable, Category = "RefinedPower")
			void calculateReactorPowerProduction();
		*/
		UFUNCTION(BlueprintCallable, Category = "RefinedPower")
			float getReactorPowerProduction();

		UFUNCTION(BlueprintCallable, Category = "RefinedPower")
			void startReactorPowerProduction();

		UFUNCTION(BlueprintCallable, Category = "RefinedPower")
			void collectInputResource(UFGFactoryConnectionComponent* inputConnection, TSubclassOf<UFGItemDescriptor> type, const int maxStorage, int& inputAmount);

	protected:
		/** Amount of power this reactor produces in MW. */
		UPROPERTY(VisibleDefaultsOnly, Category = "RefinedPower")
			float mReactorPowerProduction;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
			class UFGPowerConnectionComponent* FGPowerConnection;
};
