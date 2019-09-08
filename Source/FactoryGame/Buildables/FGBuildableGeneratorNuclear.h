// Copyright 2016-2018 Coffee Stain Studios. All Rights Reserved.

#pragma once
#include "Array.h"
#include "UObject/Class.h"

#include "CoreMinimal.h"
#include "FGBuildableGeneratorFuel.h"
#include "FGBuildableGeneratorNuclear.generated.h"

/**
 * A generator that runs on nuclear fuel and produces waste.
 */
UCLASS()
class FACTORYGAME_API AFGBuildableGeneratorNuclear : public AFGBuildableGeneratorFuel
{
	GENERATED_BODY()
public:
	// Replication
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	virtual void BeginPlay() override;

protected:
	// Begin AFGBuildableGeneratorFuel
	virtual void LoadFuel() override;
	// End AFGBuildableGeneratorFuel

	/** We can only load fuel if waste is not full */
	virtual bool CanLoadFuel() const override;

	/** Returns the inventory for waste in the nuclear generator */
	UFUNCTION( BlueprintPure, Category = "Nuclear" )
	FORCEINLINE class UFGInventoryComponent* GetWasteInventory() const { return mOutputInventory; }
private:
	/** Spent fuel rods goes here. */
	UPROPERTY( SaveGame, Replicated )
	class UFGInventoryComponent* mOutputInventory;
};
