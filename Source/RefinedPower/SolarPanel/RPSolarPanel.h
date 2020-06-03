// ProjectIcarus

#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "GameFramework/Actor.h"
#include "FGBuildableGenerator.h"
#include "FGPowerConnectionComponent.h"
#include "FGPowerInfoComponent.h"
#include "Containers/Array.h"
#include "UnrealNetwork.h"
#include "Engine.h"
#include "FGRemoteCallObject.h"
#include "RPSolarController.h"
#include "FGTimeSubsystem.h"
#include "RPLineTraceComponent.h"
#include "RPSolarPanel.generated.h"

UENUM(BlueprintType)
enum class ESolarPanelType : uint8
{
	RP_DAYONLY	UMETA(DisplayName = "Day Only"),
	RP_DAYNIGHT	UMETA(DisplayName = "Day & Night"),
	RP_NIGHT	UMETA(DisplayName = "Night")
};

UCLASS()
class REFINEDPOWER_API URPSolarPanelRCO : public UFGRemoteCallObject {
	GENERATED_BODY()

public:
	UFUNCTION(Server, WithValidation, Reliable)
		void SetPanelEnabled(ARPSolarPanel* panel, bool enabled);

	UFUNCTION(Server, WithValidation, Reliable)
		void SetMaintainMW(ARPSolarPanel* panel, float amount);

	UFUNCTION(Server, WithValidation, Reliable)
		void SetAmountToStore(ARPSolarPanel* panel, float amount);

	UPROPERTY(Replicated)
		bool bTest = true;
};

/**
Base solar panel implemtation.
Black box to the network -- is only seen as a generator.
 */
UCLASS()
class REFINEDPOWER_API ARPSolarPanel : public AFGBuildableGenerator
{
	GENERATED_BODY()
	ARPSolarPanel();
	~ARPSolarPanel();
public:
	
	virtual void BeginPlay() override;
	//virtual bool ShouldSave_Implementation() const override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	virtual void Factory_Tick(float dt) override;
	virtual void Tick(float dt) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	float GetPanelPowerOutput();
	float GetPowerOutput();
	void SetPowerOutput();

	void UpdateLineTraceRotation();

	UPROPERTY(BlueprintReadOnly, SaveGame, Replicated)
		bool mPanelEnabled = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		bool mHasBattery;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		float mMaxBatteryStorage = 10000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		float mBatteryPowerStored = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame, Replicated, Category = "RefinedPower")
		float mPercentageToStore = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame, Replicated, Category = "RefinedPower")
		float mMaintainPowerOutputAmount = 0;


protected:
	/*Max power output from this solar panel at mid-day*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		float mMaxSolarPanelProduction;

	/*Min power output from this solar panel at sunrise*/
	UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
		float mMinSolarPanelProduction;

	UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
		float mNightTimePowerReduction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		ESolarPanelType mPanelType = ESolarPanelType::RP_DAYONLY;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		bool mRotatesTowardSun = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "RefinedPower")
		int mTotalBlockingHits = 0;

	/*cached refernce to the solar controller*/
	UPROPERTY(Replicated)
	ARPSolarController* mSolarController;

	/*getter for the solar controller*/
	ARPSolarController* GetSolarController();

	void CacheTraceLineComponents();
	TArray<UActorComponent*> mCachedTraceLineComponents;
	USphereComponent* mCachedTraceLineController;

	void DetectObjectsInWay();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Solar")
		void setPanelEnabled(bool enabled);

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Solar")
		void setPanelMaintainMW(float amount);

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Solar")
		void setPanelStoreAmount(float amount);

	/*cached time subsystem to avoid getting every actor tick*/
	AFGTimeOfDaySubsystem* timeSys;

	// Used because factory tick is a dick and fires every frame regaurdless of TickInterval!
	float mDetectShadowsTimer = 0;

	FTimerHandle mSolarPanelHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UFGPowerConnectionComponent* FGPowerConnection;


};