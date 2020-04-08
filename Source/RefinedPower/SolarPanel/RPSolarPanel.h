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

	float GetPowerOutput();
	void SetPowerOutput();

	UFUNCTION(NetMulticast, Reliable)
		void Multicast_UpdateSolarPanelRotation();

	UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|Solar")
		void UpdateSolarPanelRotation(FRotator orientation);

	UPROPERTY(BlueprintReadOnly, SaveGame, Replicated)
		bool mPanelEnabled = true;

protected:
	/*Max power output from this solar panel at mid-day*/
	UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
		float mMaxSolarPanelProduction;

	/*Min power output from this solar panel at sunrise*/
	UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
		float mMinSolarPanelProduction;

	UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
		float mNightTimePowerReduction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		ESolarPanelType mPanelType = ESolarPanelType::RP_DAYONLY;

	/*cached refernce to the solar controller*/
	ARPSolarController* mSolarController;

	/*getter for the solar controller*/
	ARPSolarController* GetSolarController();

	void CachePanelStaticMesh();
	UStaticMeshComponent* mCachedPanelStaticMesh;

	void DetectObjectsInWay();

	/*cached time subsystem to avoid getting every actor tick*/
	AFGTimeOfDaySubsystem* timeSys;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UFGPowerConnectionComponent* FGPowerConnection;

};