// ILikeBanas


#include "RPWaterTurbineHologram.h"
#include "FGWaterVolume.h"
#include "util/Logging.h"
#include "Containers/UnrealString.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/WeakObjectPtr.h"

//bool ARPWaterTurbineHologram::IsValidHitResult(const FHitResult& hitResult) const {
//	Super::IsValidHitResult(hitResult);
//
//	if (hitResult.Actor.IsValid()) {
//		if (Cast<AFGWaterVolume>(hitResult.Actor)) {
//			return false;
//		}
//	}
//	return true;
//}


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

bool ARPWaterTurbineHologram::IsValidHitResult(const FHitResult& hitResult) const {
	Super::IsValidHitResult(hitResult);

	if (hitResult.Component.IsValid()) {
		AFGWaterVolume* p = (AFGWaterVolume*)(hitResult.Component).Get();
		if (p && !p->IsA((UClass*)AFGWaterVolume::StaticClass())) {
			SML::Logging::info("[RefinedPower] - Valid Water Volume");
			return true;
		}


		/*TArray<AActor*> foundWater;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFGWaterVolume::StaticClass(), foundWater);
		for(int i = 0; i < foundWater.Num(); i++)
		{
			AFGWaterVolume* water = (AFGWaterVolume*)foundWater[i];
			if (this->IsOverlappingActor(water)) {
				SML::Logging::info("[RefinedPower] - Valid Water Volume");
				return true;
			}
		}*/
	}
	SML::Logging::info("[RefinedPower] - Invalid Water Volume");
	return false;
}
//void ARPWaterTurbineHologram::CheckValidPlacement() {
//	Super::CheckValidPlacement();
//
//}