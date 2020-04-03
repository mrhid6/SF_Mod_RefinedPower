// ILikeBanas


#include "RPDSHologram.h"
#include "util/Logging.h"
#include "Containers/UnrealString.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/WeakObjectPtr.h"
#include "FGConstructDisqualifier.h"
#include "RPDysonSphere.h"

ARPDSHologram::ARPDSHologram() {

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	PrimaryActorTick.SetTickFunctionEnable(true);

	mMaxPlacementFloorAngle = 90;
}

ARPDSHologram::~ARPDSHologram() {}

void ARPDSHologram::BeginPlay() {
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARPDysonSphere::StaticClass(), mCachedDSArr);

	if (mCachedDSArr.Num() > 0) {
		mFoundInWorld = true;
	}
	else {
		mFoundInWorld = false;
	}
}


void ARPDSHologram::CheckValidPlacement() {
	Super::CheckValidPlacement();

	if (mFoundInWorld) {
		AddConstructDisqualifier(UFGCDUniqueBuilding::StaticClass());
	}
}
