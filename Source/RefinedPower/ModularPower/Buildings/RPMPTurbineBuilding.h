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

	void CalcTurbineState();

	bool CanStartSteamConsumption();
	void StartSteamConsumption();

	void ConvertSteamToRPM();

	void ReduceRPM();


	// Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category = "RefinedPower")
	int mCurrentTurbineRPM = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
	int mYellowMinTurbineRPM = 15000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
	int mYellowMaxTurbineRPM = 19999;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
	int mRedMinTurbineRPM = 20000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
	int mRedMaxTurbineRPM = 25000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
	int mSteamConsumption = 100;

	int mSteamCounter = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame, meta = (UIMin = "0", UIMax = "1.0"), Category = "RefinedPower")
	float mSteamDiscardPercent = 0.5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
	int mRPMDrag = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category = "RefinedPower")
		int mSteamConsumptionRate = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category = "RefinedPower")
		int mSteamOutputRate = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UFGInventoryComponent* mTurbineInventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		TSubclassOf<UFGItemDescriptor> mHighSteamItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		TSubclassOf<UFGItemDescriptor> mLowSteamItemClass;

	UFGPipeConnectionComponent* InputSteamPipe;
	UFGPipeConnectionComponent* OutputSteamPipe;

	int mInputInvIndex = 0;
	int mOutputInvIndex = 1;
	
};
