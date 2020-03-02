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

	protected:
		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			FVector SpinupRotation;
};
