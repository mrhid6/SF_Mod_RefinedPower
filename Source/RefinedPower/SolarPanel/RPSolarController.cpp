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
	CacheMoonSunActors();
}

void ARPSolarController::Factory_Tick(float dt)
{
	Super::Factory_Tick(dt);
	if (HasAuthority()) {
		UpdateSolarProductionScalar();
		UpdateCorrectOrientation();
	}
}

ARPSolarController* ARPSolarController::Get(UWorld* world)
{
	TArray<AActor*> temp;
	UGameplayStatics::GetAllActorsOfClass(world, ARPSolarController::StaticClass(), temp);

	if (temp.Num() > 0) {
		ARPSolarController* controller = Cast<ARPSolarController>(temp[0]);
		return controller;
	}
	else {
		SML::Logging::info("CantFind Controller!");
	}

	return nullptr;
}

// Called every frame
void ARPSolarController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



/*Function to get the sun an moon actors (assumes they are the only directional lights in the level)*/
void ARPSolarController::CacheMoonSunActors() {
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADirectionalLight::StaticClass(), moonsun);
}

TArray<AActor*> ARPSolarController::GetMoonSunActors() {
	return moonsun;
}

ADirectionalLight* ARPSolarController::GetSunActor()
{
	ADirectionalLight* sun = Cast<ADirectionalLight>(GetMoonSunActors()[1]);
	return sun;
}

ADirectionalLight* ARPSolarController::GetMoonActor()
{
	ADirectionalLight* moon = Cast<ADirectionalLight>(GetMoonSunActors()[0]);
	return moon;
}

void ARPSolarController::UpdateSolarProductionScalar() {
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
	mCurrentProductionScalar = ((2 * pct) - pow(pct, 2));
}

void ARPSolarController::UpdateCorrectOrientation() {
	FRotator rotation;
	if (timeSys->GetDayPct() > 0.01f) {
		rotation = GetSunActor()->GetActorRotation();
	}
	else {
		rotation = GetMoonActor()->GetActorRotation();
	}
	float temp = rotation.GetComponentForAxis(EAxis::Y);
	temp -= 1;
	rotation.SetComponentForAxis(EAxis::Y, temp);

	mOrientation = rotation;
}

FRotator ARPSolarController::GetOrientation() {
	return mOrientation;
}
float ARPSolarController::GetCurrectProductionScalar() {
	return mCurrentProductionScalar;
}
