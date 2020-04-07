// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "FGTimeSubsystem.h"
#include "GameFramework/Actor.h"
#include "RPSolarController.generated.h"

UCLASS()
class REFINEDPOWER_API ARPSolarController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPSolarController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*getters and setters*/
	FRotator getOrientation();
	float getCurrectProductionScalar();

	/*correct rotator for solar panel orientation*/
	FRotator orientation = FRotator(0.0, 0.0, 0.0);

protected:
	/*the current production scalar based on the solarProductionScalar function*/
	float currentProductionScalar = 0;

	/*cached sun and moon actors - [0] is moon - [1] is sun*/
	TArray<AActor*> moonsun;

	/*get sun&moon actors*/
	void getMoonSun();

private:
	void updateSolarProductionScalar();
	void updateCorrectOrientation();

	/*cached time subsystem to avoid getting every actor tick*/
	AFGTimeOfDaySubsystem* timeSys;

};
