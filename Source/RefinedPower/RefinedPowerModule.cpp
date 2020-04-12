#include "RefinedPowerModule.h"
#include "Turbine/RPTurbineBaseActor.h"
#include "Reactor/RPArcReactor.h"
#include "Reactor/RPMicroReactor.h"
#include "DysonSphere/RPDysonSphere.h"
#include "SolarPanel/RPSolarController.h"
#include "../SML/mod/hooking.h"
#include "FGGameMode.h"
#include "util/Logging.h"
#include <fstream>


void GameModePostLogin(CallScope<void(*)(AFGGameMode*, APlayerController*)>& scope, AFGGameMode* gm, APlayerController* pc) {
	if (gm->HasAuthority() && !gm->IsMainMenuGameMode()) {
		//SML::Logging::info("[RefinedPower] - Register RCO");
		gm->RegisterRemoteCallObjectClass(URPTurbineBaseRCO::StaticClass());
		gm->RegisterRemoteCallObjectClass(URPArcReactorRCO::StaticClass());
		gm->RegisterRemoteCallObjectClass(URPMicroReactorRCO::StaticClass());
		gm->RegisterRemoteCallObjectClass(URPDysonSphereRCO::StaticClass());
	}
}

void SpawnSolarController(CallScope<void(*)(AFGGameMode*, APlayerController*)>& scope, AFGGameMode* gm, APlayerController* pc) {

	if (!gm->IsMainMenuGameMode()) {
		ARPSolarController* controller = ARPSolarController::Get(gm->GetWorld());

		if (controller == nullptr) {
			SML::Logging::info("[RefinedPower] - Spawned SolarController");

			FTransform transform = FTransform();
			transform.SetScale3D(FVector(1, 1, 1));

			ARPSolarController* controller = gm->GetWorld()->SpawnActorDeferred<ARPSolarController>(ARPSolarController::StaticClass(), transform, NULL, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			UGameplayStatics::FinishSpawningActor(controller, transform);

			SML::Logging::info(TCHAR_TO_UTF8(*controller->GetTransform().ToString()));
		}
		else {
			SML::Logging::info("[RefinedPower] - Skipped Spawning SolarController");
		}
	}
}

void FRefinedPowerModule::StartupModule() {
	SUBSCRIBE_METHOD("?PostLogin@AFGGameMode@@UEAAXPEAVAPlayerController@@@Z", AFGGameMode::PostLogin, &GameModePostLogin);
	//SUBSCRIBE_METHOD("?PostLogin@AFGGameMode@@UEAAXPEAVAPlayerController@@@Z", AFGGameMode::PostLogin, &SpawnSolarController);
}

IMPLEMENT_GAME_MODULE(FRefinedPowerModule, RefinedPower);
