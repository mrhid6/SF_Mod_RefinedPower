// ILikeBanas


#include "RPMPGeneratorBuilding.h"
#include "../RPMPPlatform.h"


ARPMPGeneratorBuilding::ARPMPGeneratorBuilding() {

}

void ARPMPGeneratorBuilding::BeginPlay() {
	Super::BeginPlay();

	if (HasAuthority()) {
		CacheTurbineBuilding();
	}
}

void ARPMPGeneratorBuilding::UpdateDependantBuildings() {
	Super::UpdateDependantBuildings();
	CacheTurbineBuilding();
}

void ARPMPGeneratorBuilding::CacheTurbineBuilding() {
	ARPMPPlatform* platform;
	GetAttachedPlatform(platform);

	if (platform) {
		TArray<AActor*> platformBuildings = platform->GetAttachedMPBuildings();
		bool foundTurbine = false;
		for (AActor* building : platformBuildings) {
			if (building->IsA(ARPMPTurbineBuilding::StaticClass())) {
				mAttachedTurbine = Cast<ARPMPTurbineBuilding>(building);
				SML::Logging::info("[RefinedPower] - Got Turbine!");
				foundTurbine = true;
				break;
			}
		}

		if (foundTurbine == false) {
			mAttachedTurbine = nullptr;
		}
	}
	else {
		SML::Logging::info("[RefinedPower] - CantFind Platform!");
	}
}