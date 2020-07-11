// ILikeBanas


#include "RPMPPlacementComponent.h"
#include "RPMPBuilding.h"
#include "Buildings/RPMPBoilerBuilding.h"
#include "Buildings/RPMPHeaterBuilding.h"
#include "Buildings/RPMPTurbineBuilding.h"
#include "Buildings/RPMPGeneratorBuilding.h"
#include "Buildings/RPMPCoolingBuilding.h"

// Sets default values for this component's properties
URPMPPlacementComponent::URPMPPlacementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URPMPPlacementComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void URPMPPlacementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


bool URPMPPlacementComponent::IsOccupied() {
	const TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes = TArray< TEnumAsByte< EObjectTypeQuery > >{ EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery2 };
	TArray< AActor*> ActorsToIgnore;
	TArray< AActor*> OutActors;


	UKismetSystemLibrary::SphereOverlapActors(this, GetComponentLocation(), 10, ObjectTypes, ARPMPBuilding::StaticClass(), ActorsToIgnore, OutActors);
	bool resBool = false;

	for (AActor* Actor : OutActors) {
		ARPMPBuilding* Building = Cast<ARPMPBuilding>(Actor);

		switch (mBuildingType) {
		case EMPPlatformBuildingType::MP_Boiler:
			if (Building->IsA(ARPMPBoilerBuilding::StaticClass())) {
				resBool = true;
			}
			break;

		case EMPPlatformBuildingType::MP_Heater:
			if (Building->IsA(ARPMPHeaterBuilding::StaticClass())) {
				resBool = true;
			}
			break;

		case EMPPlatformBuildingType::MP_Turbine:
			if (Building->IsA(ARPMPTurbineBuilding::StaticClass())) {
				resBool = true;
			}
			break;

		case EMPPlatformBuildingType::MP_Generator:
			if (Building->IsA(ARPMPGeneratorBuilding::StaticClass())) {
				resBool = true;
			}
			break;
		case EMPPlatformBuildingType::MP_Cooler:
			if (Building->IsA(ARPMPCoolingBuilding::StaticClass())) {
				resBool = true;
			}
			break;
		}
	}

	return resBool;
}

