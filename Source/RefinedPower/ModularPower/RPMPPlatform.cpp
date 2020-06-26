#include "RPMPPlatform.h"
#include "FGFactoryConnectionComponent.h"
#include "UnrealNetwork.h"
#include "FGPowerInfoComponent.h"
#include "util/Logging.h"
#include "FGPowerConnectionComponent.h"
#include "FGPlayerController.h"


ARPMPPlatform::ARPMPPlatform() {

	SetReplicates(true);
	bReplicates = true;
	mFactoryTickFunction.bCanEverTick = true;

}

ARPMPPlatform::~ARPMPPlatform() {}

void ARPMPPlatform::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}

void ARPMPPlatform::BeginPlay() {
	Super::BeginPlay();

	if (HasAuthority()) {
		SetupInitalPlacement();
	}
}

void ARPMPPlatform::Tick(float dt) {
	Super::Tick(dt);

	if (HasAuthority() && mConnectionToCoreUpdated) {
		mConnectionToCoreUpdated = false;
		Multicast_CoreConnectionUpdated();
	}
}

void ARPMPPlatform::Factory_Tick(float dt) {
	Super::Factory_Tick(dt);

	if (HasAuthority()) {

		ForceNetUpdate();
	}
}

void ARPMPPlatform::SetupInitalPlacement() {
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

void ARPMPPlatform::SetupConnectionToCore(ARPMPCore* MPCore){

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

void ARPMPPlatform::Multicast_CoreConnectionUpdated_Implementation(){
	OnRep_CoreConnectionUpdated();
}
