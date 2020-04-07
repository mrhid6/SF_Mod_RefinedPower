// ILikeBanas

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
#include "RPSolarPanel.generated.h"

/**
Base solar panel implemtation.
Black box to the network -- is only seen as a generator.
 */

UENUM(BlueprintType)
enum class EPanelType : uint8
{
	RP_DAYONLY	UMETA(DisplayName = "Day Only"),
	RP_DAYNIGHT	UMETA(DisplayName = "Day & Night")
};

UCLASS()
class REFINEDPOWER_API URPSolarPanelRCO : public UFGRemoteCallObject {
	GENERATED_BODY()

public:
	UFUNCTION(Server, WithValidation, Reliable)
		void SetPanelEnabled(ARPSolarPanel* panel, bool enabled);
};

UCLASS()
class REFINEDPOWER_API ARPSolarPanel : public AFGBuildableGenerator
{
	GENERATED_BODY()
	ARPSolarPanel();
	~ARPSolarPanel();
public:
	
	virtual void BeginPlay() override;
	virtual bool ShouldSave_Implementation() const override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UPROPERTY(SaveGame)
	uint32 mPanelEnabled;

	UFUNCTION(BlueprintPure, Category = "RefinedPower|SolarPanel")
	void updatePanelRotation();

	UFUNCTION(BlueprintPure, Category = "RefinedPower|SolarPanel")
	void updateSupportRotation();


protected:
	/*Max power output from this solar panel at mid-day*/
	UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
	float mMaxSolarPanelProduction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFGPowerConnectionComponent* FGPowerConnection;

	/*Type of panel this is - Whether or not this solar panel can generate power at night (ie. from the moonlight)*/
	UPROPERTY(EditDefaultsOnly, Category = "RefinedPower|Solar")
	EPanelType mPanelType = EPanelType::RP_DAYONLY;

	/*cached refernce to the solar controller*/
	AActor* solarController;

	/*getter for the solar controller - only one in world instance*/
	void getSolarController();

	/*components*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFGPowerConnectionComponent* FGPowerConnection;


};
