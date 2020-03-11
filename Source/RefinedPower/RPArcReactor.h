#pragma once

#include "CoreMinimal.h"
#include "Core.h"
#include "RPReactorBaseActor.h"
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
class REFINEDPOWER_API ARPArcReactor : public ARPReactorBaseActor
{
	GENERATED_BODY()

	ARPArcReactor();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Factory_Tick(float dt) override;
	virtual void Tick(float dt) override;

	public:
		/*########## Main operational functions ##########*/

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			void ToggleLight();

			void CalcResourceState();

			void CalcReactorState();

			void ReduceResourceAmounts();

			void UpdatePowerProducedThisCycle(float dT);
		/*################################################*/

		/*########## Utility functions ##########*/

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			void SetReactorState(EReactorState state);

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			EReactorState GetReactorState();

			void IncreaseSpinAmount();

			void DecreaseSpinAmount();

			void CalcSpinningState();

			void RenderStateSpunDown();

			void RenderStateSpunUp();

			void ProduceMW();

			void UpdateParticleVariables();

			void CalcAudio();
		
		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			int getReactorSpinAmount();

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			int getInput1Amount();

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			int getInput2Amount();

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			int getPipeInputAmount();

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			bool isSoundEnabled();

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			void setSoundEnabled(bool enabled);

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			bool isParticlesEnabled();

		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			void setParticlesEnabled(bool enabled);

		/*#######################################*/

	protected:

		/*################### Partcile Variables ####################*/

		UPROPERTY(VisibleAnywhere, SaveGame, Replicated, Category = "RefinedPower")
			FVector SpinupRotation = FVector(0.0f, 0.0f, 0.0f);

		UPROPERTY(VisibleAnywhere, SaveGame, Replicated, Category = "RefinedPower")
			float SpinupOpacity= 0.0f;

		UPROPERTY(VisibleAnywhere, SaveGame, Replicated, Category = "RefinedPower")
			int ReactorSpinAmount = 0;

		UPROPERTY(EditDefaultsOnly, SaveGame, Replicated, Category = "RefinedPower")
			bool particlesEnabled = false;

		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
			UParticleSystemComponent* PlasmaParticles;

		bool mUpdateParticleVars;

		/*#######################################*/

		/*################### Reactor State ####################*/

		UPROPERTY(VisibleAnywhere, SaveGame, Replicated, Category = "RefinedPower")
			EReactorState ReactorState = EReactorState::RP_State_SpunDown;
		UPROPERTY(VisibleAnywhere, SaveGame, Replicated, Category = "RefinedPower")
			EReactorState ReactorPrevState = EReactorState::RP_State_SpunDown;

		/*#######################################*/

		/*################### Input Varibles ####################*/

		/*Amount stored in item input 1*/
		UPROPERTY(VisibleAnywhere, SaveGame, Replicated, Category = "RefinedPower")
			int InputConveyor1Amount = 0;

		/*Amount stored in item input 2*/
		UPROPERTY(VisibleAnywhere, SaveGame, Replicated, Category = "RefinedPower")
			int InputConveyor2Amount = 0;

		/*Amount stored in pipe input*/
		UPROPERTY(VisibleAnywhere, SaveGame, Replicated, Category = "RefinedPower")
			int InputPipe1Amount = 0;

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

		/*Amount of power produced since the last item consumed*/
		UPROPERTY(VisibleAnywhere, Category = "RefinedPower")
			float PowerProducedThisCycle = 0.0f;

		/*How much power we produce per item consumption*/
		UPROPERTY(VisibleAnywhere, Category = "RefinedPower")
			float PowerValuePerCycle = 30000.0f;

		/*#######################################*/

		/*################### Input Connection Components ####################*/

		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
			TSubclassOf<UFGItemDescriptor> Conveyor1InputClass;
		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
			TSubclassOf<UFGItemDescriptor> Conveyor2InputClass;
		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
			TSubclassOf<UFGItemDescriptor> Pipe1InputClass;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
			UFGFactoryConnectionComponent* InputConveyor1;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
			UFGFactoryConnectionComponent* InputConveyor2;
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RefinedPower")
			UFGPipeConnectionComponent* InputPipe;

		/*#######################################*/

		/*################### Misc Components ####################*/
		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			USpotLightComponent* SpotLight;

		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			UAudioComponent* ArcReactorSound;

		

		/*#######################################*/
};
