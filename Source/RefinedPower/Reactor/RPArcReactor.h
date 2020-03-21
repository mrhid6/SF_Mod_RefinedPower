#pragma once

#include "CoreMinimal.h"
#include "Core.h"
#include "Buildables/FGBuildableGeneratorFuel.h"
#include "FGGameUserSettings.h"
#include "FGRemoteCallObject.h"
#include "RPArcReactor.generated.h"

UENUM(BlueprintType)
enum class EReactorState : uint8
{
	RP_State_SpinUp 	UMETA(DisplayName = "SpinUp"),
	RP_State_Producing 	UMETA(DisplayName = "Producing"),
	RP_State_SpinDown 	UMETA(DisplayName = "SpinDown"),
	RP_State_SpunDown 	UMETA(DisplayName = "SpunDown")
};

UCLASS()
class REFINEDPOWER_API URPArcReactorRCO : public UFGRemoteCallObject {
	GENERATED_BODY()

public:
	UFUNCTION(Server, WithValidation, Reliable)
		void SetParticlesEnabled(ARPArcReactor* reactor, bool enabled);

	UFUNCTION(Server, WithValidation, Reliable)
		void SetSoundEnabled(ARPArcReactor* reactor, bool enabled);

	UPROPERTY(Replicated)
		bool bTest = true;
};

UCLASS()
class REFINEDPOWER_API ARPArcReactor : public AFGBuildableGeneratorFuel
{
	GENERATED_BODY()

	ARPArcReactor();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Factory_Tick(float dt) override;
	virtual void Tick(float dt) override;

	virtual bool CanStartPowerProduction_Implementation() const override;

	public:
		/*#### start Main operational functions ####*/

		void CalcReactorState();

		/*#### end main operaitonal functions ####*/

		/*####Utility functions ####*/

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			void SetReactorState(EReactorState state);

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			EReactorState GetReactorState();

		UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|Reactor|ArcReactor")
			void StartSpinupSound();

		UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|Reactor|ArcReactor")
			void StartProducingSound();

		UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|Reactor|ArcReactor")
			void StartShutdownSound();

		void IncreaseSpinAmount();

		void DecreaseSpinAmount();

		void CalcSpinningState();

		void RenderStateSpunDown();

		void RenderStateSpunUp();

		void UpdateParticleVariables();

		void CalcAudio();



		/*#### End Util functions ####*/

		/*#### Start getters and setters ####*/

		UFUNCTION(BlueprintPure, Category = "RefinedPower|Reactor|ArcReactor")
			int getReactorSpinAmount();

		UFUNCTION(BlueprintPure, Category = "RefinedPower|Reactor|ArcReactor")
			bool isSoundEnabled();

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			void setSoundEnabled(bool enabled);

		UFUNCTION(BlueprintPure, Category = "RefinedPower|Reactor|ArcReactor")
			bool isParticlesEnabled();

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			void setParticlesEnabled(bool enabled);

		UFUNCTION(BlueprintPure, Category = "RefinedPower|Reactor|ArcReactor")
			int getReactorCores();

		UFUNCTION(BlueprintPure, Category = "RefinedPower|Reactor|ArcReactor")
			float getReactorCoolantInternal();

		UFUNCTION(BlueprintPure, Category = "RefinedPower|Reactor|ArcReactor")
			float getReactorCoolantInternalMax();

		UFUNCTION(BlueprintPure, Category = "RefinedPower|Reactor|ArcReactor")
			float getReactorCoolantBuffer();

		UFUNCTION(BlueprintPure, Category = "RefinedPower|Reactor|ArcReactor")
			float getReactorCoolantBufferMax();

		/*#### End getters and setters #####*/

		UPROPERTY(EditDefaultsOnly, SaveGame, Category = "RefinedPower")
			bool mParticlesEnabled = false;

		UPROPERTY(EditDefaultsOnly, SaveGame, Category = "RefinedPower")
			bool mReactorSoundEnabled = true;


		bool mUpdateParticleVars;
		bool mUpdateAudio;

	protected:

		/*#### Start Particle vars ####*/

		UPROPERTY(VisibleAnywhere, SaveGame, Category = "RefinedPower")
			FVector mSpinupRotation = FVector(0.0f, 0.0f, 0.0f);

		UPROPERTY(VisibleAnywhere, SaveGame, Category = "RefinedPower")
			float mSpinupOpacity= 0.0f;

		UPROPERTY(VisibleAnywhere, SaveGame, Replicated, Category = "RefinedPower")
			float mReactorSpinAmount = 0;

		UPROPERTY(VisibleAnywhere, SaveGame, Replicated, Category = "RefinedPower")
			int32 RPFuelInvIndex = mFuelInventoryIndex;

		UPROPERTY(VisibleAnywhere, SaveGame, Replicated, Category = "RefinedPower")
			int32 RPCoolantInvIndex = mSupplementalInventoryIndex;

		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
			UParticleSystemComponent* PlasmaParticles;

		/*#### End Particle vars ####*/

		/*#### Start Reactor State vars ####*/

		UPROPERTY(VisibleAnywhere, SaveGame, Replicated, Category = "RefinedPower")
			EReactorState ReactorState = EReactorState::RP_State_SpunDown;

		/*#### End Reactor State vars ####*/

		/*#### Start Input vars ####*/

		/*Min amount of resources required to start production*/
		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
			int MinStartAmount = 1500;

		/*When the resources fall below this amount, the reactor will shut off*/
		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
			int MinStopAmount = 1000;

		/*#### End input vars ####*/

		/* #### Start input connection components ####*/

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
			UFGFactoryConnectionComponent* InputConveyor;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
			UFGPipeConnectionComponent* InputPipe;

		/*#### end input connection components ####*/

		/*#### start misc componenents ####*/

		UPROPERTY(VisibleAnywhere, Category = "RefinedPower")
			USpotLightComponent* SpotLight;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
			UAudioComponent* ArcReactorSound;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
			class UFGPowerConnectionComponent* FGPowerConnection;
		
		/*#### end misc components ####*/
};
