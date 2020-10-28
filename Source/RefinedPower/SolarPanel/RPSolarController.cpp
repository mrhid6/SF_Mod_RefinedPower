// ILikeBanas


#include "RPSolarController.h"
#include "FGTimeSubsystem.h"
#include <cmath>
#include "UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARPSolarController::ARPSolarController()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    SetReplicates(true);
    bReplicates = true;
}

ARPSolarController::~ARPSolarController()
{
}

// Called when the game starts or when spawned
void ARPSolarController::BeginPlay()
{
    Super::BeginPlay();

    CacheInstancedMeshPool();

    timeSys = AFGTimeOfDaySubsystem::Get(this);
    CacheMoonSunActors();

    GetWorld()->GetTimerManager().SetTimer(mRotationTimerHandle, this, &ARPSolarController::UpdateSolarPanelsRotation,
                                           5.0f, true);
    GetWorld()->GetTimerManager().SetTimer(mScalarTimerHandle, this, &ARPSolarController::UpdateSolarProductionScalar,
                                           0.5f, true);
}

ARPSolarController* ARPSolarController::Get(UWorld* world)
{
    TArray<AActor*> temp;
    UGameplayStatics::GetAllActorsOfClass(world, ARPSolarController::StaticClass(), temp);

    if (temp.Num() > 0)
    {
        ARPSolarController* controller = Cast<ARPSolarController>(temp[0]);
        return controller;
    }
    else
    {
        SML::Logging::info("[RP] - CantFind Controller!");
    }

    return nullptr;
}

// Called every frame
void ARPSolarController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARPSolarController::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
    Super::EndPlay(endPlayReason);
}


/*Function to get the sun an moon actors (assumes they are the only directional lights in the level)*/
void ARPSolarController::CacheMoonSunActors()
{
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADirectionalLight::StaticClass(), moonsun);
}

TArray<AActor*> ARPSolarController::GetMoonSunActors()
{
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

void ARPSolarController::UpdateSolarProductionScalar()
{
    if (HasAuthority())
    {
        float pct = 0;
        if (timeSys && timeSys->IsDay())
        {
            pct = timeSys->GetDayPct();
        }
        else if (timeSys && timeSys->IsNight())
        {
            pct = timeSys->GetNightPct();
        }

        /*controls the solar panel production ratio based on TOD
        For a more detailed analysis of this function: https://www.desmos.com/calculator/mfzrurdb5e
        */
        mCurrentProductionScalar = ((2 * pct) - pow(pct, 2));
    }
}

void ARPSolarController::UpdateCorrectOrientation()
{
    FRotator rotation;
    if (timeSys)
    {
        if (timeSys->GetDayPct() > 0.01f)
        {
            rotation = GetSunActor()->GetActorRotation();
        }
        else
        {
            rotation = GetMoonActor()->GetActorRotation();
        }
        float temp = rotation.GetComponentForAxis(EAxis::Y);
        temp -= 1;
        rotation.SetComponentForAxis(EAxis::Y, temp);

        mOrientation = rotation;
    }
    else
    {
        mOrientation = FRotator(0, 0, 0);
    }
}

FRotator ARPSolarController::GetOrientation()
{
    return mOrientation;
}

float ARPSolarController::GetCurrectProductionScalar()
{
    return mCurrentProductionScalar;
}

void ARPSolarController::CacheInstancedMeshPool()
{
    FName tag = FName(TEXT("PanelMeshPool"));
    TArray<UActorComponent*> comps = GetComponentsByTag(UHierarchicalInstancedStaticMeshComponent::StaticClass(), tag);
    mPanelMeshPool = Cast<UHierarchicalInstancedStaticMeshComponent>(comps[0]);

    tag = FName(TEXT("SupportMeshPool"));
    comps = GetComponentsByTag(UHierarchicalInstancedStaticMeshComponent::StaticClass(), tag);
    mSupportMeshPool = Cast<UHierarchicalInstancedStaticMeshComponent>(comps[0]);
}


void ARPSolarController::UpdateSolarPanelsRotation()
{
    if (mPanelMeshPool == nullptr) return;
    if (HasAuthority())
    {
        UpdateCorrectOrientation();
    }

    FQuat rotation = GetOrientation().Quaternion();

    FRotator rot2 = GetOrientation();
    rot2.Pitch = 0;
    rot2.Roll = 0;

    FQuat rotation2 = rot2.Quaternion();

    int meshCount = mPanelMeshPool->GetInstanceCount();

    for (int i = 0; i < meshCount; i++)
    {
        FTransform origInstTransform1;
        FTransform origInstTransform2;

        mPanelMeshPool->GetInstanceTransform(i, origInstTransform1, true);
        mSupportMeshPool->GetInstanceTransform(i, origInstTransform2, true);

        origInstTransform1.SetRotation(rotation);
        origInstTransform2.SetRotation(rotation2);

        mPanelMeshPool->UpdateInstanceTransform(i, origInstTransform1, true, false, true);
        mSupportMeshPool->UpdateInstanceTransform(i, origInstTransform2, true, false, true);
    }

    FinishUpdates();
}

//Instance Static Mesh Stuff:

void ARPSolarController::SpawnIM(FTransform initPanelTransform, FTransform initSupportTransform, int actorId)
{
    //SML::Logging::info(TCHAR_TO_UTF8(*initialTransform.ToString()));
    int32 index = mPanelMeshPool->AddInstanceWorldSpace(initPanelTransform);
    mSupportMeshPool->AddInstanceWorldSpace(initSupportTransform);

    IdToInstanceMapping.Add(actorId, index);
    IdBuffer.Add(actorId);
    FinishUpdates();
}

void ARPSolarController::DestroyIM(int actorId)
{
    uint32 indexToRemove = IdToInstanceMapping[actorId];
    uint32 actorLastIndex = IdBuffer.Num() - 1;
    uint32 actorToMove = IdBuffer[actorLastIndex];
    IdBuffer[indexToRemove] = actorToMove;
    IdToInstanceMapping[actorToMove] = IdToInstanceMapping[actorId];
    IdToInstanceMapping.Remove(actorId);
    IdBuffer.RemoveAt(actorLastIndex);

    mPanelMeshPool->RemoveInstance(indexToRemove);
    mSupportMeshPool->RemoveInstance(indexToRemove);
}

void ARPSolarController::FinishUpdates()
{
    mPanelMeshPool->MarkRenderStateDirty();
    mSupportMeshPool->MarkRenderStateDirty();
}

void ARPSolarController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ARPSolarController, mOrientation);
    DOREPLIFETIME(ARPSolarController, mCurrentProductionScalar);
}
