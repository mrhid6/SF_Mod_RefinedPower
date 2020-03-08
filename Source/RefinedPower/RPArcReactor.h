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

			void IncreaseSpinAmount();

			void DecreaseSpinAmount();

			void CalcSpinningState();

			void RenderStateSpunDown();

			void RenderStateSpunUp();

			void ProduceMW();

			void RenderReactorState();

			void SetReactorPlasmaColor();

			void CalcAudio();
		
		UFUNCTION(BlueprintCallable, Category = "RefinedPower|Reactor|ArcReactor")
			int getReactorSpinAmount();
		/*#######################################*/

	protected:

		/*################### Partcile Variables ####################*/

		UPROPERTY(VisibleAnywhere, Replicated, Category = "RefinedPower")
			FVector SpinupRotation;

		UPROPERTY(VisibleAnywhere, Replicated, Category = "RefinedPower")
			float SpinupOpacity;

		UPROPERTY(VisibleAnywhere, Replicated, Category = "RefinedPower")
			int ReactorSpinAmount;

		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
			bool particlesEnabled;

		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
			UParticleSystemComponent* PlasmaParticles;

		/*#######################################*/

		/*################### Reactor State ####################*/

		UPROPERTY(VisibleAnywhere, Replicated, Category = "RefinedPower")
			EReactorState ReactorState;
		UPROPERTY(VisibleAnywhere, Replicated, Category = "RefinedPower")
			EReactorState ReactorPrevState;

		/*#######################################*/

		/*################### Input Varibles ####################*/

		UPROPERTY(VisibleAnywhere, Replicated, Category = "RefinedPower")
			int InputConveyor1Amount;
		UPROPERTY(VisibleAnywhere, Replicated, Category = "RefinedPower")
			int InputConveyor2Amount;
		UPROPERTY(VisibleAnywhere, Replicated, Category = "RefinedPower")
			int InputPipe1Amount;


		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
			int MaxResourceAmount;
		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
			int MinStartAmount;
		UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
			int MinStopAmount;

		UPROPERTY(VisibleAnywhere, Category = "RefinedPower")
			float PowerProducedThisCycle;
		UPROPERTY(VisibleAnywhere, Category = "RefinedPower")
			float PowerValuePerCycle;

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
