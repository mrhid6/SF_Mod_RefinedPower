#include "RPMPPlatform.h"
#include "FGFactoryConnectionComponent.h"
#include "UnrealNetwork.h"
#include "FGPowerInfoComponent.h"
#include "util/Logging.h"
#include "FGPowerConnectionComponent.h"
#include "RPMPPlacementComponent.h"
#include "FGPlayerController.h"
#include "RPMPBuilding.h"

#include "Buildings/RPMPBoilerBuilding.h"
#include "Buildings/RPMPHeaterBuilding.h"
#include "Buildings/RPMPTurbineBuilding.h"
#include "Buildings/RPMPGeneratorBuilding.h"
#include "Buildings/RPMPCoolingBuilding.h"


ARPMPPlatform::ARPMPPlatform()
{
    mPowerInfo = CreateDefaultSubobject<UFGPowerInfoComponent>(TEXT("mPowerInfo"));

    SetReplicates(true);
    bReplicates = true;
    mFactoryTickFunction.bCanEverTick = true;
}

ARPMPPlatform::~ARPMPPlatform()
{
}

void ARPMPPlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ARPMPPlatform::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {

        UActorComponent* tempPowerComp = GetComponentByClass(UFGPowerConnectionComponent::StaticClass());

        if(tempPowerComp)
        {
            UFGPowerConnectionComponent* powerComp = Cast<UFGPowerConnectionComponent>(tempPowerComp);
            powerComp->SetPowerInfo(GetPowerInfo());
        }
        
        UpdatePlatformAttachments();
    }
}

void ARPMPPlatform::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    Super::EndPlay(endPlayReason);
}

void ARPMPPlatform::Tick(float dt)
{
    Super::Tick(dt);
}

void ARPMPPlatform::Factory_Tick(float dt)
{
    Super::Factory_Tick(dt);

    if (HasAuthority())
    {
    }
}

void ARPMPPlatform::UpdatePlatformAttachments()
{
    TArray<AActor*> buildings = GetAttachedMPBuildings();

    for (auto TempBuilding : buildings)
    {
        ARPMPBuilding* Building = Cast<ARPMPBuilding>(TempBuilding);

        Building->UpdateDependantBuildings();
    }
}

TArray<AActor*> ARPMPPlatform::GetAttachedMPBuildings()
{
    TArray<UActorComponent*> placementComps = GetComponentsByClass(URPMPPlacementComponent::StaticClass());


    TArray<AActor*> resActors;

    for (UActorComponent* comp : placementComps)
    {
        URPMPPlacementComponent* placementComp = Cast<URPMPPlacementComponent>(comp);

        if (placementComp->mAttachedBuilding)
        {
            resActors.AddUnique(placementComp->mAttachedBuilding);
        }
    };

    return resActors;
}

void ARPMPPlatform::AttachBuildingToComp(AActor* Actor)
{
    ARPMPBuilding* Building = Cast<ARPMPBuilding>(Actor);

    if (Building)
    {
        URPMPPlacementComponent* placementComp = nullptr;

        if (Building->IsA(ARPMPBoilerBuilding::StaticClass()))
        {
            //SML::Logging::info("[RefinedPower] - Attached to comp Boiler");
            placementComp = GetPlacementComponent(EMPPlatformBuildingType::MP_Boiler);
        }

        if (Building->IsA(ARPMPHeaterBuilding::StaticClass()))
        {
            //SML::Logging::info("[RefinedPower] - Attached to comp heater");
            placementComp = GetPlacementComponent(EMPPlatformBuildingType::MP_Heater);
        }

        if (Building->IsA(ARPMPTurbineBuilding::StaticClass()))
        {
            //SML::Logging::info("[RefinedPower] - Attached to comp turbine");
            placementComp = GetPlacementComponent(EMPPlatformBuildingType::MP_Turbine);
        }

        if (Building->IsA(ARPMPGeneratorBuilding::StaticClass()))
        {
            //SML::Logging::info("[RefinedPower] - Attached to comp generator");
            placementComp = GetPlacementComponent(EMPPlatformBuildingType::MP_Generator);
        }

        if (Building->IsA(ARPMPCoolingBuilding::StaticClass()))
        {
            //SML::Logging::info("[RefinedPower] - Attached to comp cooler");
            placementComp = GetPlacementComponent(EMPPlatformBuildingType::MP_Cooler);
        }

        if (placementComp != nullptr)
        {
            placementComp->mAttachedBuilding = Building;
        }
    }
}

void ARPMPPlatform::DetachBuildingFromComp(AActor* Actor)
{
    ARPMPBuilding* Building = Cast<ARPMPBuilding>(Actor);

    if (Building)
    {
        URPMPPlacementComponent* placementComp = nullptr;

        if (Building->IsA(ARPMPBoilerBuilding::StaticClass()))
        {
            //SML::Logging::info("[RefinedPower] - Attached to comp Boiler");
            placementComp = GetPlacementComponent(EMPPlatformBuildingType::MP_Boiler);
        }

        if (Building->IsA(ARPMPHeaterBuilding::StaticClass()))
        {
            //SML::Logging::info("[RefinedPower] - Attached to comp heater");
            placementComp = GetPlacementComponent(EMPPlatformBuildingType::MP_Heater);
        }

        if (Building->IsA(ARPMPTurbineBuilding::StaticClass()))
        {
            //SML::Logging::info("[RefinedPower] - Attached to comp turbine");
            placementComp = GetPlacementComponent(EMPPlatformBuildingType::MP_Turbine);
        }

        if (Building->IsA(ARPMPGeneratorBuilding::StaticClass()))
        {
            //SML::Logging::info("[RefinedPower] - Attached to comp generator");
            placementComp = GetPlacementComponent(EMPPlatformBuildingType::MP_Generator);
        }

        if (Building->IsA(ARPMPCoolingBuilding::StaticClass()))
        {
            //SML::Logging::info("[RefinedPower] - Attached to comp cooler");
            placementComp = GetPlacementComponent(EMPPlatformBuildingType::MP_Cooler);
        }

        if (placementComp != nullptr)
        {
            placementComp->mAttachedBuilding = nullptr;
        }
    }
}

URPMPPlacementComponent* ARPMPPlatform::GetPlacementComponent(EMPPlatformBuildingType type)
{
    TArray<UActorComponent*> comps = GetComponentsByClass(URPMPPlacementComponent::StaticClass());

    if (comps.Num() > 0)
    {
        for (UActorComponent* comp : comps)
        {
            URPMPPlacementComponent* placementComp = Cast<URPMPPlacementComponent>(comp);
            if (placementComp->mBuildingType == type)
            {
                return placementComp;
            }
        }
    }

    return nullptr;
}
