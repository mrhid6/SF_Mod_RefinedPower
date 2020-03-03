#pragma once

#include "CoreMinimal.h"
#include "RPReactorBaseActor.h"
#include "RPArcReactor.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPArcReactor : public ARPReactorBaseActor
{
	GENERATED_BODY()

	ARPArcReactor();

	virtual void BeginPlay() override;

	public:
		UFUNCTION(BlueprintCallable, Category = "RefinedPower")
			void CalcResourceState();
		UFUNCTION(BlueprintCallable, Category = "RefinedPower")
			void CalcReactorState();
		UFUNCTION(BlueprintCallable, Category = "RefinedPower")
			void ReduceResourceAmounts();

	protected:
		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			FVector SpinupRotation;
		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			float SpinupOpacity;
		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			uint8 ReactorState;
		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			uint64 ReactorSpinAmount;
		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			uint8 ReactorPrevState;
		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			uint64 InputConveyor1Amount;
		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			uint64 InputConveyor2Amount;
		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			uint64 InputPipe1Amount;
		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			float BasePowerProduction;
		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			uint64 MaxResourceAmount;
		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			uint64 MaxStartAmount;
		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			uint64 MaxStopAmount;
		UPROPERTY(VisibleAnywhere, Category = "RefinedPower")
			UFGFactoryConnectionComponent* InputConveyor1;
		UPROPERTY(VisibleAnywhere, Category = "RefinedPower")
			UFGFactoryConnectionComponent* InputConveyor2;
		UPROPERTY(VisibleAnywhere, Category = "RefinedPower")
			UFGFactoryConnectionComponent* InputPipe1;
};
