// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealNetwork.h"
#include "FGResourceNode.h"
#include "RPWaterTurbineNode.generated.h"

UCLASS(Blueprintable)
class REFINEDPOWER_API ARPWaterTurbineNode : public AFGResourceNode
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPWaterTurbineNode();

	virtual FText GetLookAtDecription_Implementation(class AFGCharacterPlayer* byCharacter, const FUseState& state) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, SaveGame)
		bool mHasTurbine = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

};
