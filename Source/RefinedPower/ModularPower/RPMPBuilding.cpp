#include "RPMPBuilding.h"
#include "FGFactoryConnectionComponent.h"
#include "UnrealNetwork.h"
#include "FGPowerInfoComponent.h"
#include "util/Logging.h"
#include "FGPowerConnectionComponent.h"
#include "FGPlayerController.h"


ARPMPBuilding::ARPMPBuilding() {

	SetReplicates(true);
	bReplicates = true;
	mFactoryTickFunction.bCanEverTick = true;

}

ARPMPBuilding::~ARPMPBuilding() {}

void ARPMPBuilding::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void ARPMPBuilding::BeginPlay() {
	Super::BeginPlay();

	if (HasAuthority()) {
		SetupInitalPlacement();
	}
}

void ARPMPBuilding::Tick(float dt) {
	Super::Tick(dt);

	if (HasAuthority() && mConnectionToCoreUpdated) {
		mConnectionToCoreUpdated = false;
		Multicast_CoreConnectionUpdated();
	}
}

void ARPMPBuilding::Factory_Tick(float dt) {
	Super::Factory_Tick(dt);

	if (HasAuthority()) {

		ForceNetUpdate();
	}
}

void ARPMPBuilding::SetupInitalPlacement() {
	if (mConnectedToCore == false) {
		SML::Logging::info("[RefinedPower] - MPBuilding: Inital Setup");
		TArray<AActor*> AllCoresInWorld;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARPMPCore::StaticClass(), AllCoresInWorld);

		for (int i = 0; i < AllCoresInWorld.Num(); i++)
		{
			ARPMPCore* MPCore = (ARPMPCore*)AllCoresInWorld[i];
			MPCore->CacheNearbyMPBuildings();
			MPCore->ConfigureNearbyMPBuildings();
		}
	}
}

void ARPMPBuilding::SetupConnectionToCore(ARPMPCore* MPCore){

	if (MPCore != nullptr) {
		SML::Logging::info("[RefinedPower] - MPBuilding: Connected To Core");
		mPowerInfo = MPCore->GetPowerInfo();
		mConnectedToCore = true;
	}
	else {
		mConnectedToCore = false;
	}

	mConnectionToCoreUpdated = true;
}

void ARPMPBuilding::Multicast_CoreConnectionUpdated_Implementation(){
	OnRep_CoreConnectionUpdated();
}
