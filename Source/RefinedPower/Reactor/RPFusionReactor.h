// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Buildables/FGBuildableGeneratorFuel.h"
#include "FGFactoryConnectionComponent.h"
#include "FGPipeConnectionComponent.h"
#include "FGRemoteCallObject.h"
#include "FGPowerInfoComponent.h"
#include "FGPowerConnectionComponent.h"
#include "FGInventoryComponent.h"
#include "FGInventoryLibrary.h"
#include "util/Logging.h"
#include "UnrealNetwork.h"
#include "RPFusionReactor.generated.h"


UENUM(BlueprintType)
enum class EFusionReactorState : uint8
{
	RP_State_Charging 	UMETA(DisplayName = "Charging"),
	RP_State_SpinUp 	UMETA(DisplayName = "SpinUp"),
	RP_State_Producing 	UMETA(DisplayName = "Producing"),
	RP_State_SpinDown 	UMETA(DisplayName = "SpinDown"),
	RP_State_SpunDown 	UMETA(DisplayName = "SpunDown")
};

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPFusionReactor : public AFGBuildableGeneratorFuel
{
	GENERATED_BODY()

public:
	ARPFusionReactor();
	~ARPFusionReactor();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Factory_Tick(float dt) override;
	virtual void Tick(float dt) override;

	virtual bool CanStartPowerProduction_Implementation() const override;


	// Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, SaveGame, Category = "RefinedPower")
		float mSpinRate = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, SaveGame, Category = "RefinedPower")
		EFusionReactorState mReactorState = EFusionReactorState::RP_State_SpunDown;

	bool TriggerSpinRateChanged = false;

	float mChargeAmountPerTick = 1000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, SaveGame, Category = "RefinedPower")
	float mChargeAmount = 0.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
	float mMaxCharge = 5000000;


	// Functions

	UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower")
		void OnSpinRateChanged();

	UFUNCTION(Client, Reliable)
		void Client_SpinRateChanged();

	void ChargeReactor();
	void ResetReactorCharge();
	void IncreaseSpinUp();
	void DecreaseSpinUp();



	UPROPERTY(Replicated)
		int mCachedReactorCoresAmount;

	UPROPERTY(Replicated)
		int mCachedReactorCoresMax;
	
	UPROPERTY(Replicated)
		float mCachedCoolantAmount;

	UFUNCTION(BlueprintPure)
		int getReactorCores();

	UFUNCTION(BlueprintPure)
		int getReactorCoresMax();
	
	UFUNCTION(BlueprintPure)
		float getReactorCoolantInternal();

	UFUNCTION(BlueprintPure)
		float getReactorCoolantInternalMax();

	UFUNCTION(BlueprintPure)
	float getReactorCoolantBuffer();

	UFUNCTION(BlueprintPure)
	float getReactorCoolantBufferMax();

	void cacheFuelAmounts();

	/*Min amount of resources required to start production*/
	UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
		int mMinStartAmount = 40;

	/*When the resources fall below this amount, the reactor will shut off*/
	UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
		int mMinStopAmount = 20;

	// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
		UFGFactoryConnectionComponent* InputConveyor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
		UFGPipeConnectionComponent* InputPipe;

	UPROPERTY(VisibleAnywhere, Category = "RefinedPower")
		USpotLightComponent* SpotLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
		UAudioComponent* FusionReactorSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UFGPowerConnectionComponent* FGPowerConnection;
	
};
