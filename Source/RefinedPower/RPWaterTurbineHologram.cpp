// ILikeBanas


#include "RPWaterTurbineHologram.h"
#include "FGWaterVolume.h"
#include "util/Logging.h"
#include "Containers/UnrealString.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/WeakObjectPtr.h"
#include "Components/BoxComponent.h"

ARPWaterTurbineHologram::ARPWaterTurbineHologram() {
	WaterTest = CreateDefaultSubobject<UBoxComponent>(TEXT("WaterTest"));
	WaterTest->SetupAttachment(RootComponent);

	FVector boxSize = FVector(100, 100, 100);
	WaterTest->SetBoxExtent(boxSize);

	WaterTest->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

bool ARPWaterTurbineHologram::IsValidHitResult(const FHitResult& hitResult) const {
	Super::IsValidHitResult(hitResult);

	return SetOverlapWaterVolume();
}

bool ARPWaterTurbineHologram::SetOverlapWaterVolume() const {

	ARPWaterTurbineHologram* ptr = const_cast<ARPWaterTurbineHologram*>(this);

	TArray<AActor*> foundWater;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFGWaterVolume::StaticClass(), foundWater);

	for (int i = 0; i < foundWater.Num(); i++)
	{
		AFGWaterVolume* water = (AFGWaterVolume*)foundWater[i];

		// This WORKS!
		if (WaterTest->IsOverlappingActor(water)) {
			SML::Logging::info("[RefinedPower] - Valid Water Volume");


			// CANT SET THIS!!!
			ptr->FoundWater = water;
			return true;
		}
	}

	ptr->FoundWater = nullptr;
	return false;
}



void ARPWaterTurbineHologram::SetHologramLocationAndRotation(const FHitResult& hitResult) {
	
	// Some weird shit going on here! probs IF statement?

	if (!FoundWater) {
		Super::SetHologramLocationAndRotation(hitResult);
	}
	else {
		SML::Logging::info("[RefinedPower] - Set Location to WATER?");
		FVector WaterOrigin;
		FVector WaterBounds;
		FoundWater->GetActorBounds(false, WaterOrigin, WaterBounds);

		float WaterZ = WaterOrigin.Z;
		WaterZ += WaterBounds.Z;

		FVector location = FVector(hitResult.ImpactPoint.X, hitResult.ImpactPoint.Y, WaterZ);

		this->SetActorLocation(location, false);
	}
}


void ARPWaterTurbineHologram::BeginPlay() {
	Super::BeginPlay();

	testScript();
}

void ARPWaterTurbineHologram::testScript() {
	//Grabs class names of valid hit results -- remove on release
	/*SML::Logging::info("[RefinedPower] - ValidHitClasses: ");
	AddValidHitClass(AFGWaterVolume::StaticClass());
	for (int i = 0; i < mValidHitClasses.Num(); i++){
		const char* className = "NULL";
		const char* objectName = "NULL";
		UObject* const pCastedToUObject = (UObject*)(mValidHitClasses[i]);

		if (pCastedToUObject)
		{
			className = TCHAR_TO_UTF8(*pCastedToUObject->GetClass()->GetName());
			objectName = TCHAR_TO_UTF8(*pCastedToUObject->GetName());
		}

		SML::Logging::info("[RefinedPower] - ", className, " ", objectName);
	}*/
}
//void ARPWaterTurbineHologram::CheckValidPlacement() {
//	Super::CheckValidPlacement();
//
//}