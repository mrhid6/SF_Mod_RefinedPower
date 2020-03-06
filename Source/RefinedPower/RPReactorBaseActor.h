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

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor")
			float getBaseReactorPowerProduction();

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor")
			void startReactorPowerProduction();

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor")
			void setBaseReactorPowerProduction(float p);

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor")
			int collectInputResource(UFGFactoryConnectionComponent* inputConnection, TSubclassOf<UFGItemDescriptor> type, const int maxStorage, int& inputAmount);

		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
			bool mReactorSoundEnabled;

		UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|Reactor")
			void StartSpinupSound();
		UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|Reactor")
			void StartProducingSound();
		UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|Reactor")
			void StartShutdownSound();

	protected:

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
			class UFGPowerConnectionComponent* FGPowerConnection;
};
