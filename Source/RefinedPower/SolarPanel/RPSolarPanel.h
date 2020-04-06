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



protected:
	/*Max power output from this solar panel at mid-day*/
	UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
		float mMaxSolarPanelProduction;

	/*Whether or not this solar panel can generate power at night (ie. from the moonlight)*/
	UPROPERTY(EditDefaultsOnly, Category = "RefinedPower")
		bool mIsNightProducer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UFGPowerConnectionComponent* FGPowerConnection;

	/*cached refernce to the solar controller*/
	AActor* solarController;

	/*getter for the solar controller*/
	AActor* getSolarController();
};
