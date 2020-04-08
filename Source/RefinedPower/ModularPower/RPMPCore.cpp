#include "RPMPCore.h"
#include "FGFactoryConnectionComponent.h"
#include "UnrealNetwork.h"
#include "FGPowerInfoComponent.h"
#include "util/Logging.h"
#include "FGPowerConnectionComponent.h"
#include "FGInventoryComponent.h"
#include "FGInventoryLibrary.h"
#include "FGPlayerController.h"
#include "RPMPBuilding.h"


ARPMPCore::ARPMPCore() {
	FGPowerConnection = CreateDefaultSubobject<UFGPowerConnectionComponent>(TEXT("FGPowerConnection"));
	FGPowerConnection->SetupAttachment(RootComponent);

	mPowerModuleRange = CreateDefaultSubobject<USphereComponent>(TEXT("PowerModuleRange"));
	mPowerModuleRange->SetupAttachment(RootComponent);

	mPowerModuleRange->InitSphereRadius(1000.0f);

	SetReplicates(true);
	bReplicates = true;
	mFactoryTickFunction.bCanEverTick = true;

}

ARPMPCore::~ARPMPCore() {}

void ARPMPCore::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void ARPMPCore::BeginPlay() {
	Super::BeginPlay();

	if (HasAuthority()) {
		FGPowerConnection->SetPowerInfo(GetPowerInfo());
		CacheNearbyMPBuildings();
		ConfigureNearbyMPBuildings();
	}
}

void ARPMPCore::EndPlay(const EEndPlayReason::Type endPlayReason) {

	if (endPlayReason == EEndPlayReason::Destroyed) {
		UnconfigureNearbyMPBuildings();
	}
	Super::EndPlay(endPlayReason);
}

void ARPMPCore::Tick(float dt) {
	Super::Tick(dt);
}

void ARPMPCore::Factory_Tick(float dt) {
	Super::Factory_Tick(dt);

	if (HasAuthority()) {

		ForceNetUpdate();
	}
}

void ARPMPCore::CacheNearbyMPBuildings()
{
	const FVector ActorLocation = GetActorLocation();

	const TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes = TArray< TEnumAsByte< EObjectTypeQuery > >{ EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery2 };
	TArray< AActor*> ActorsToIgnore = TArray< AActor*>{ };
	const float radius = mPowerModuleRange->GetUnscaledSphereRadius();

	UKismetSystemLibrary::SphereOverlapActors(this, ActorLocation, radius, ObjectTypes, ARPMPBuilding::StaticClass(), ActorsToIgnore, mCachedNearbyMPBuildings);

}

void ARPMPCore::ConfigureNearbyMPBuildings()
{
	for (AActor* mpBuilding : mCachedNearbyMPBuildings) {
		ARPMPBuilding* RPMPBuilding = (ARPMPBuilding*)mpBuilding;
		RPMPBuilding->SetupConnectionToCore(this);
	}
}

void ARPMPCore::UnconfigureNearbyMPBuildings()
{
	for (AActor* mpBuilding : mCachedNearbyMPBuildings) {
		ARPMPBuilding* RPMPBuilding = (ARPMPBuilding*)mpBuilding;
		RPMPBuilding->SetupConnectionToCore(nullptr);
	}
}