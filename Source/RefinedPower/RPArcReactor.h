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
	virtual void Factory_Tick(float dT) override;

	public:
		/*########## Main operational functions ##########*/
		UFUNCTION(BlueprintCallable, Category = "RefinedPower|ArcReactor")
			void ToggleLight();
		UFUNCTION(BlueprintCallable, Category = "RefinedPower|ArcReactor")
			void CalcResourceState();
		UFUNCTION(BlueprintCallable, Category = "RefinedPower|ArcReactor")
			void CalcReactorState();
		UFUNCTION(BlueprintCallable, Category = "RefinedPower|ArcReactor")
			void ReduceResourceAmounts();
		/*################################################*/

		/*########## Utility functions ##########*/
		UFUNCTION(BlueprintCallable, Category = "RefinedPower|ArcReactor")
			void SetReactorState(EReactorState state);
		UFUNCTION(BlueprintCallable, Category = "RefinedPower|ArcReactor")
			void IncreaseSpinAmount();
		UFUNCTION(BlueprintCallable, Category = "RefinedPower|ArcReactor")
			void DecreaseSpinAmount();
		UFUNCTION(BlueprintCallable, Category = "RefinedPower|ArcReactor")
			void CalcSpinningState();
		UFUNCTION(BlueprintCallable, Category = "RefinedPower|ArcReactor")
			void RenderStateSpunDown();
		UFUNCTION(BlueprintCallable, Category = "RefinedPower|ArcReactor")
			void RenderStateSpunUp();
		UFUNCTION(BlueprintCallable, Category = "RefinedPower|ArcReactor")
			void ProduceMW();
		UFUNCTION(BlueprintCallable, Category = "RefinedPower|ArcReactor")
			void RenderReactorState();
		UFUNCTION(BlueprintCallable, Category = "RefinedPower|ArcReactor")
			void SetReactorPlasmaColor();
		UFUNCTION(BlueprintCallable, Category = "RefinedPower|ArcReactor")
			void CalcAudio();
		UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|ArcReactor")
			void StartSpinupSound();
		UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|ArcReactor")
			void StartProducingSound();
		UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|ArcReactor")
			void StartShutdownSound();
		/*#######################################*/

	protected:
		UPROPERTY(EditAnywhere, Replicated, Category = "RefinedPower|ArcReactor")
			FVector SpinupRotation;
		UPROPERTY(EditAnywhere, Replicated, Category = "RefinedPower|ArcReactor")
			float SpinupOpacity;
		UPROPERTY(EditAnywhere, Replicated, Category = "RefinedPower|ArcReactor")
			EReactorState ReactorState;
		UPROPERTY(EditAnywhere, Replicated, Category = "RefinedPower|ArcReactor")
			int ReactorSpinAmount;
		UPROPERTY(EditAnywhere, Replicated, Category = "RefinedPower|ArcReactor")
			EReactorState ReactorPrevState;
		UPROPERTY(EditAnywhere, Replicated, Category = "RefinedPower")
			int InputConveyor1Amount;
		UPROPERTY(EditAnywhere, Replicated, Category = "RefinedPower")
			int InputConveyor2Amount;
		UPROPERTY(EditAnywhere, Replicated, Category = "RefinedPower")
			int InputPipe1Amount;
		UPROPERTY(EditAnywhere, Category = "RefinedPower|ArcReactor")
			int MaxResourceAmount;
		UPROPERTY(EditAnywhere, Category = "RefinedPower|ArcReactor")
			int MinStartAmount;
		UPROPERTY(EditAnywhere, Category = "RefinedPower|ArcReactor")
			int MinStopAmount;
		UPROPERTY(EditAnywhere, Category = "RefinedPower|ArcReactor")
			TSubclassOf<UFGItemDescriptor> Conveyor1InputClass;
		UPROPERTY(EditAnywhere, Category = "RefinedPower|ArcReactor")
			TSubclassOf<UFGItemDescriptor> Conveyor2InputClass;
		UPROPERTY(EditAnywhere, Category = "RefinedPower|ArcReactor")
			TSubclassOf<UFGItemDescriptor> Pipe1InputClass;
		UPROPERTY(VisibleAnywhere, Category = "RefinedPower")
			UFGFactoryConnectionComponent* InputConveyor1;
		UPROPERTY(VisibleAnywhere, Category = "RefinedPower")
			UFGFactoryConnectionComponent* InputConveyor2;
		UPROPERTY(VisibleAnywhere, Category = "RefinedPower")
			UFGFactoryConnectionComponent* InputPipe1;
		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			USpotLightComponent* SpotLight;
		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			bool particlesEnabled;
		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			UParticleSystemComponent* PlasmaParticles;
		UPROPERTY(EditAnywhere, Category = "RefinedPower")
			UAudioComponent* ArcReactorSound;
};
