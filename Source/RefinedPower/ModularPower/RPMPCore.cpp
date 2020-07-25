#include "RPMPCore.h"
#include "FGFactoryConnectionComponent.h"
#include "UnrealNetwork.h"
#include "FGPowerInfoComponent.h"
#include "util/Logging.h"
#include "FGPowerConnectionComponent.h"
#include "FGInventoryComponent.h"
#include "FGInventoryLibrary.h"
#include "FGPlayerController.h"
#include "RPMPPlatform.h"


ARPMPCore::ARPMPCore()
{
    FGPowerConnection = CreateDefaultSubobject<UFGPowerConnectionComponent>(TEXT("FGPowerConnection"));
    FGPowerConnection->SetupAttachment(RootComponent);

    mPowerModuleRange = CreateDefaultSubobject<USphereComponent>(TEXT("PowerModuleRange"));
    mPowerModuleRange->SetupAttachment(RootComponent);

    mPowerModuleRange->InitSphereRadius(1000.0f);

    SetReplicates(true);
    bReplicates = true;
    mFactoryTickFunction.bCanEverTick = true;
}

ARPMPCore::~ARPMPCore()
{
}

void ARPMPCore::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ARPMPCore::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        FGPowerConnection->SetPowerInfo(GetPowerInfo());
        CacheNearbyMPBuildings();
        ConfigureNearbyMPBuildings();
    }
}

void ARPMPCore::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    if (endPlayReason == EEndPlayReason::Destroyed)
    {
        UnconfigureNearbyMPBuildings();
    }
    Super::EndPlay(endPlayReason);
}

void ARPMPCore::Tick(float dt)
{
    Super::Tick(dt);
}

void ARPMPCore::Factory_Tick(float dt)
{
    Super::Factory_Tick(dt);

    if (HasAuthority())
    {
        UpdatePowerData();
    }
}

void ARPMPCore::CacheNearbyMPBuildings()
{
    const FVector ActorLocation = GetActorLocation();

    const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = TArray<TEnumAsByte<EObjectTypeQuery>>{
        EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery2
    };
    TArray<AActor*> ActorsToIgnore = TArray<AActor*>{};
    const float radius = mPowerModuleRange->GetUnscaledSphereRadius();

    UKismetSystemLibrary::SphereOverlapActors(this, ActorLocation, radius, ObjectTypes, ARPMPPlatform::StaticClass(),
                                              ActorsToIgnore, mCachedNearbyMPPlatforms);
}

void ARPMPCore::ConfigureNearbyMPBuildings()
{
    for (AActor* mpBuilding : mCachedNearbyMPPlatforms)
    {
        ARPMPPlatform* RPMPPlatform = (ARPMPPlatform*)mpBuilding;
        RPMPPlatform->SetupConnectionToCore(this);
    }
}

void ARPMPCore::UnconfigureNearbyMPBuildings()
{
    for (AActor* mpBuilding : mCachedNearbyMPPlatforms)
    {
        ARPMPPlatform* RPMPPlatform = (ARPMPPlatform*)mpBuilding;
        RPMPPlatform->SetupConnectionToCore(nullptr);
    }
}

void ARPMPCore::UpdatePowerData()
{
    float PowerProduction = 0.0f;

    for (AActor* mpBuilding : mCachedNearbyMPPlatforms)
    {
        ARPMPPlatform* RPMPPlatform = (ARPMPPlatform*)mpBuilding;

        if (RPMPPlatform->mPlatformType == EMPPlatformType::MP_ConverterPlatform)
        {
            float PlatformOutput = RPMPPlatform->GetPowerInfo()->GetBaseProduction();
            if (PlatformOutput > 0 && PlatformOutput < 12000)
            {
                SML::Logging::info(PlatformOutput);
                PowerProduction += PlatformOutput;
            }
        }
    }

    GetPowerInfo()->SetBaseProduction(PowerProduction);
}
