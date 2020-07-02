// ILikeBanas


#include "RPMPGeneratorBuilding.h"
#include "../RPMPPlatform.h"


ARPMPGeneratorBuilding::ARPMPGeneratorBuilding() {

}

void ARPMPGeneratorBuilding::BeginPlay() {
	if (HasAuthority()) {
		CacheTurbineBuilding();
	}
}

void ARPMPGeneratorBuilding::CacheTurbineBuilding() {
	ARPMPPlatform* platform;
	GetAttachedPlatform(platform);

	if (platform) {
		TArray<AActor*> platformBuildings = platform->GetAttachedMPBuildings();

		for (AActor* building : platformBuildings) {
			if (building->IsA(ARPMPTurbineBuilding::StaticClass())) {
				mAttachedTurbine = Cast<ARPMPTurbineBuilding>(building);
				SML::Logging::info("[RefinedPower] - Got Turbine!");
			}
		}
	}
}