#pragma once

#include "CoreMinimal.h"
#include "Core.h"
#include "Buildables/FGBuildableGeneratorFuel.h"
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

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			void ToggleLight();

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

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			int getReactorSpinAmount();

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			bool isSoundEnabled();

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			void setSoundEnabled(bool enabled);

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			bool isParticlesEnabled();

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			void setParticlesEnabled(bool enabled);

		/*#### End getters and setters #####*/

	protected:

		/*#### Start Particle vars ####*/

		UPROPERTY(VisibleAnywhere, SaveGame, Category = "RefinedPower")
			FVector SpinupRotation = FVector(0.0f, 0.0f, 0.0f);

		UPROPERTY(VisibleAnywhere, SaveGame, Category = "RefinedPower")
			float SpinupOpacity= 0.0f;

		UPROPERTY(VisibleAnywhere, SaveGame, Replicated, Category = "RefinedPower")
			int ReactorSpinAmount = 0;

		UPROPERTY(EditDefaultsOnly, SaveGame, Category = "RefinedPower")
			bool particlesEnabled = false;

		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
			UParticleSystemComponent* PlasmaParticles;

		bool mUpdateParticleVars;

		/*#### End Particle vars ####*/

		/*#### Start Reactor State vars ####*/

		UPROPERTY(VisibleAnywhere, SaveGame, Replicated, Category = "RefinedPower")
			EReactorState ReactorState = EReactorState::RP_State_SpunDown;

		/*#### End Reactor State vars ####*/

		/*#### Start Input vars ####*/

		/*Max amount of items we can store*/
		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
			int MaxResourceAmount = 2000;

		/*Max Amount of fluids we can store -- this is the max amount a pipe can output in 2 minutes*/
		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
			int MaxFluidAmount = 6000;

		/*Min amount of resources required to start production*/
		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
			int MinStartAmount = 1500;

		/*When the resources fall below this amount, the reactor will shut off*/
		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
			int MinStopAmount = 1000;

		UPROPERTY(EditDefaultsOnly, SaveGame, Category = "RefinedPower")
			bool mReactorSoundEnabled = true;

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

		UPROPERTY(VisibleAnywhere, Category = "RefinedPower")
			UAudioComponent* ArcReactorSound;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
			class UFGPowerConnectionComponent* FGPowerConnection;
		
		/*#### end misc components ####*/
};
