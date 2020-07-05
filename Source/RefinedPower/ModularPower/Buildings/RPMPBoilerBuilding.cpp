// ILikeBanas


#include "RPMPBoilerBuilding.h"

ARPMPBoilerBuilding::ARPMPBoilerBuilding() {
	SetReplicates(true);
	bReplicates = true;
	mFactoryTickFunction.SetTickFunctionEnable(true);
	mFactoryTickFunction.bCanEverTick = true;
}

void ARPMPBoilerBuilding::BeginPlay() {
	Super::BeginPlay();

	if (HasAuthority()) {
		CacheHeaterBuilding();
	}

}
void ARPMPBoilerBuilding::Factory_Tick(float dt) {
	Super::Factory_Tick(dt);
	if (HasAuthority()) {
		
	}

}
void ARPMPBoilerBuilding::Tick(float dt) {
	Super::Tick(dt);
}


void ARPMPBoilerBuilding::UpdateDependantBuildings() {
	Super::UpdateDependantBuildings();
	CacheHeaterBuilding();
}


void ARPMPBoilerBuilding::CacheHeaterBuilding() {
	ARPMPPlatform* platform;
	GetAttachedPlatform(platform);

	if (platform) {
		TArray<AActor*> platformBuildings = platform->GetAttachedMPBuildings();
		bool foundHeater = false;
		for (AActor* building : platformBuildings) {
			if (building->IsA(ARPMPHeaterBuilding::StaticClass())) {
				mAttachedHeater = Cast<ARPMPHeaterBuilding>(building);
				SML::Logging::info("[RefinedPower] - Got Heater!");
				foundHeater = true;
				break;
			}
		}

		if (foundHeater == false) {
			mAttachedHeater = nullptr;
			SML::Logging::info("[RefinedPower] - No Heater!");
		}
	}
	else {
		SML::Logging::info("[RefinedPower] - CantFind Platform!");
	}
}