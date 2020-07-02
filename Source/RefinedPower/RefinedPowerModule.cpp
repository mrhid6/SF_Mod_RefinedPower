#include "RefinedPowerModule.h"
#include "Turbine/RPTurbineBaseActor.h"
#include "Reactor/RPArcReactor.h"
#include "Reactor/RPMicroReactor.h"
#include "DysonSphere/RPDysonSphere.h"
#include "SolarPanel/RPSolarController.h"
#include "SolarPanel/RPSolarPanel.h"
#include "ModularPower/Buildings/RPMPTurbineBuilding.h"
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
		gm->RegisterRemoteCallObjectClass(URPSolarPanelRCO::StaticClass());
		gm->RegisterRemoteCallObjectClass(URPMPTurbineBuildingRCO::StaticClass());
	}
}

void FRefinedPowerModule::StartupModule() {
	SUBSCRIBE_METHOD(AFGGameMode::PostLogin, &GameModePostLogin);
}

IMPLEMENT_GAME_MODULE(FRefinedPowerModule, RefinedPower);
