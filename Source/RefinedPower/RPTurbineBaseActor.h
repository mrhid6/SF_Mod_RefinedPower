#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "GameFramework/Actor.h"
#include "FGBuildableGenerator.h"
#include "FGPowerConnectionComponent.h"
#include "Containers/Array.h"
#include "UnrealNetwork.h"
#include "Engine.h"
#include "FGSaveInterface.h"
#include "FGRemoteCallObject.h"
#include "RPTurbineBaseActor.generated.h"

UENUM(BlueprintType)
enum class ETurbineType : uint8
{
	RP_Wind 	UMETA(DisplayName = "Wind"),
	RP_Water 	UMETA(DisplayName = "Water")
};

UCLASS()
class REFINEDPOWER_API URPTurbineBaseRCO : public UFGRemoteCallObject {
	GENERATED_BODY()
	
public:
	UFUNCTION(Server, WithValidation, Reliable)
		void SetTurbineEnabled(ARPTurbineBaseActor* turbine, bool enabled);
		
};

UCLASS()
class REFINEDPOWER_API ARPTurbineBaseActor : public AFGBuildableGenerator {
	GENERATED_BODY()
public:

	ARPTurbineBaseActor();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
	virtual bool ShouldSave_Implementation() const override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	void calculateTurbinePowerProduction();
	float getTurbineBasePowerProduction();
	float getTurbineHeightPowerProduction();
	void setTurbinePowerOutput();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	float getTurbineActualPowerProduction();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	float getMaxTurbinePowerProduction();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	TArray< ARPTurbineBaseActor*> getNearbyWindTurbines();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	int getNearbyWindTurbinesCount();

	void updateNearbyWindTurbineCount();

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	void setTurbineEnabled(bool turbineEnabled);

	UFUNCTION(BlueprintCallable, Category = "RefinedPower|Turbine")
	bool isTurbineEnabled();

	UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|Turbine")
		void updateTurbineParticleState();


	/** Is turbine enabled or disabled */
	UPROPERTY(SaveGame, Replicated, meta = (NoAutoJson = true))
		uint32  mTurbineEnabled;

protected:

	float mTurbinePowerProduction;

	/** Max power this turbine can produce */
	UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
	float mMaxTurbinePowerProduction;

	/** Is this turbine based on height eg higher height = more power */
	UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
	bool mIsHeightBasedTurbine;

	/** The type of Turbine, Wind Or Water */
	UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
	ETurbineType mTurbineType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFGPowerConnectionComponent* FGPowerConnection;

	int mWindTurbinesInArea;
	
	/** The max amount of other wind turbines in the area */
	UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
	int mMaxWindTurbinesInArea;

	void calcNearbyWindTurbines();

};
