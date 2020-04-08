// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "FGTimeSubsystem.h"
#include "Buildables/FGBuildableFactory.h"
#include "GameFramework/Actor.h"
#include "util/Logging.h"
#include "RPSolarController.generated.h"

UCLASS()
class REFINEDPOWER_API ARPSolarController : public AFGBuildableFactory
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPSolarController();

	// Called when in range (Not needed)
	void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	void BeginPlay() override;

	// Called Every tick
	void Factory_Tick(float dt) override;

	static ARPSolarController* Get(UWorld* world);

	/*getters and setters*/
	FRotator GetOrientation();
	float GetCurrectProductionScalar();

protected:
	/*the current production scalar based on the solarProductionScalar function*/
	float mCurrentProductionScalar = 0;

	/*correct rotator for solar panel orientation*/
	FRotator mOrientation = FRotator(0.0,0.0,0.0);

	/*cached sun and moon actors - [0] is moon - [1] is sun*/
	TArray<AActor*> moonsun;

	TArray<AActor*> GetMoonSunActors();

	ADirectionalLight* GetSunActor();
	ADirectionalLight* GetMoonActor();

private:

	/*cache sun&moon actors*/
	void CacheMoonSunActors();

	void UpdateSolarProductionScalar();
	void UpdateCorrectOrientation();

	/*cached time subsystem to avoid getting every actor tick*/
	AFGTimeOfDaySubsystem* timeSys;

};
