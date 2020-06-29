// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "ModularPower/RPMPBuilding.h"
#include "FGPipeConnectionComponent.h"
#include "RPMPTurbineBuilding.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPMPTurbineBuilding : public ARPMPBuilding
{
	GENERATED_BODY()

public:
	ARPMPTurbineBuilding();

	virtual void BeginPlay() override;
	virtual void Factory_Tick(float dt) override;
	virtual void Tick(float dt) override;

	// Functions
	void CacheConnections();
	void CollectSteam(float dt);


	// Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
	int mCurrentTurbineRPM = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
	int mMaxTurbineRPM = 1000;

	int mCurrentSteamValue = 0;
	int mMaxSteamValue = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UFGInventoryComponent* mTurbineInventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		TSubclassOf<UFGItemDescriptor> mSteamItemClass;

	UFGPipeConnectionComponent* InputSteamPipe;
	UFGPipeConnectionComponent* OutputSteamPipe;

	int mInputInvIndex = 0;
	int mOutputInvIndex = 1;
	
};
