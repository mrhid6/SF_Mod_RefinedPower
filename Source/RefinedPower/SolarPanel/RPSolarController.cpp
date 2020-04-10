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

	MeshPool = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("MeshPool"));
	MeshPool->SetupAttachment(RootComponent);
	
	// Stuff i tried to get it to render instances.
	MeshPool->SetWorldLocation(GetActorLocation());

	MeshPool->SetBoundsScale(500000.0f);
	MeshPool->bNeverDistanceCull = true;
	MeshPool->bAllowCullDistanceVolume = false;
	MeshPool->bUseAsOccluder = false;

	MeshPool->InstancingRandomSeed = 1;

	//

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/RefinedPower/Models/SolarPanelMk2/SolarPanel_Panels.SolarPanel_Panels'"));
	UStaticMesh* Asset = MeshAsset.Object;

	MeshPool->SetStaticMesh(Asset);
}

// Called when the game starts or when spawned
void ARPSolarController::BeginPlay()
{
	Super::BeginPlay();

	MeshPool->SetVisibilitySML(true, true);
	MeshPool->Activate();

	MeshPool->AddInstance(FTransform());

	timeSys = AFGTimeOfDaySubsystem::Get(this);
	CacheMoonSunActors();
	GetWorld()->GetTimerManager().SetTimer(mRotationTimerHandle, this, &ARPSolarController::UpdateSolarPanelsRotation, 5.0f, true);
}

/*void ARPSolarController::Factory_Tick(float dt)
{
	Super::Factory_Tick(dt);
	if (HasAuthority()) {
		UpdateSolarProductionScalar();
		UpdateCorrectOrientation();
	}
}*/

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


void ARPSolarController::UpdateSolarPanelsRotation() {


	FQuat rotation = GetOrientation().Quaternion();

	int meshCount = MeshPool->GetInstanceCount();
	SML::Logging::info("Updating all instances: ", meshCount);

	for (int i = 0; i < meshCount; i++) {
		FTransform origInstTransform;
		MeshPool->GetInstanceTransform(i, origInstTransform);
		origInstTransform.SetRotation(rotation);

		SML::Logging::info(TCHAR_TO_UTF8(*origInstTransform.ToString()));

		MeshPool->UpdateInstanceTransform(i, origInstTransform, true, true, true);
	}

	FinishUpdates();
}

//Instance Static Mesh Stuff:

void ARPSolarController::SpawnIM(FTransform initialTransform, int actorId) {
	SML::Logging::info(TCHAR_TO_UTF8(*initialTransform.ToString()));
	int32 index = MeshPool->AddInstance(initialTransform);
	IdToInstanceMapping.Add(actorId, index);
	IdBuffer.Add(actorId);
	FinishUpdates();
}

void ARPSolarController::DestroyIM(int actorId) {
	uint32 indexToRemove = IdToInstanceMapping[actorId];
	uint32 actorLastIndex = IdBuffer.Num() - 1;
	uint32 actorToMove = IdBuffer[actorLastIndex];
	IdBuffer[indexToRemove] = actorToMove;
	IdToInstanceMapping[actorToMove] = IdToInstanceMapping[actorId];
	IdToInstanceMapping.Remove(actorId);
	IdBuffer.RemoveAt(actorLastIndex);
}

void ARPSolarController::FinishUpdates() {
	MeshPool->MarkRenderStateDirty();
}