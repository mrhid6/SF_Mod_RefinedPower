#pragma once
#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "GameFramework/Actor.h"
#include "FGBuildablePipelineAttachment.h"
#include "FGPowerConnectionComponent.h"
#include "Containers/Array.h"
#include "UnrealNetwork.h"
#include "RPPipelineTurbine.generated.h"

UCLASS()
class REFINEDPOWER_API ARPPipelineTurbine : public AFGBuildablePipelineAttachment {
	GENERATED_BODY()
public:

	/* THIS CLASS DOESN'T WORK YET, GAME ISSUES */
	ARPPipelineTurbine();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditDefaultsOnly)
		class UFGPowerConnectionComponent* FGPowerConnection;

	/** Is turbine enabled or disabled */
	UPROPERTY(EditDefaultsOnly, SaveGame, Replicated, Category = "RefinedPower")
		bool mTurbineEnabled;

	UPROPERTY(EditDefaultsOnly)
		UFGPipeConnectionComponent* InputPipe1;

	UPROPERTY(EditDefaultsOnly)
		UFGPipeConnectionComponent* OutputPipe1;

};