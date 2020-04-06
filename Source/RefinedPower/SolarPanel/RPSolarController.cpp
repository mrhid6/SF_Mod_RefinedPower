// ILikeBanas


#include "RPSolarController.h"
#include "FGTimeSubsystem.h"
#include <cmath>
#include "Kismet/GameplayStatics.h"

// Sets default values
ARPSolarController::ARPSolarController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	bReplicates = true;
}

// Called when the game starts or when spawned
void ARPSolarController::BeginPlay()
{
	Super::BeginPlay();
	timeSys = AFGTimeOfDaySubsystem::Get(this);
}

// Called every frame
void ARPSolarController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	updateSolarProductionScalar();
	updateCorrectOrientation();
}

/*Function to get the sun an moon actors (assumes they are the only directional lights in the level)*/
void ARPSolarController::getMoonSun() {
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADirectionalLight::StaticClass(), moonsun);
}

void ARPSolarController::updateSolarProductionScalar() {
	float pct = 0;
	if (timeSys && timeSys->IsDay()) {
		pct = timeSys->GetDayPct();
	}
	else if (timeSys && timeSys->IsNight()) {
		pct = timeSys->GetNightPct();
	}

	/*controls the solar panel production ratio based on TOD
	For a more detailed analysis of this function: https://www.desmos.com/calculator/mfzrurdb5e
	*/
	currentProductionScalar = (pow(-pct,2) + (2 * pct));
}

void ARPSolarController::updateCorrectOrientation() {
	FRotator rotation;
	if (timeSys->GetDayPct() > 0.01f) {
		rotation = moonsun[0]->GetActorRotation();
	}
	else {
		rotation = moonsun[1]->GetActorRotation();
	}
	float temp = rotation.GetComponentForAxis(EAxis::Y);
	temp -= 1;
	rotation.SetComponentForAxis(EAxis::Y, temp);

	orientation = rotation;
}

FRotator ARPSolarController::getOrientation() {
	return orientation;
}
float ARPSolarController::getCurrectProductionScalar() {
	return currentProductionScalar;
}
