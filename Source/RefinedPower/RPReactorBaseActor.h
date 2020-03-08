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

			void startReactorPowerProduction();

			void setBaseReactorPowerProduction(float p);

			int collectInputResource(UFGFactoryConnectionComponent* inputConnection, TSubclassOf<UFGItemDescriptor> type, const int maxStorage, int& inputAmount);
			int collectInputFluidResource(float dt, UFGPipeConnectionComponent* inputConnection, TSubclassOf<UFGItemDescriptor> type, const int maxStorage, int& inputAmount);

		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower|Reactor")
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
