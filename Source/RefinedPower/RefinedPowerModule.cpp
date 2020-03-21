#include "RefinedPowerModule.h"
#include "Turbine/RPTurbineBaseActor.h"
#include "../SML/mod/hooking.h"
#include "FGGameMode.h"
#include <fstream>


void GameModePostLogin(CallScope<void(*)(AFGGameMode*, APlayerController*)>& scope, AFGGameMode* gm, APlayerController* pc) {
	if (gm->HasAuthority() && !gm->IsMainMenuGameMode()) {
		gm->RegisterRemoteCallObjectClass(URPTurbineBaseRCO::StaticClass());
	}
}

void FRefinedPowerModule::StartupModule() {
	SUBSCRIBE_METHOD("?PostLogin@AFGGameMode@@UEAAXPEAVAPlayerController@@@Z", AFGGameMode::PostLogin, &GameModePostLogin)
}

IMPLEMENT_GAME_MODULE(FRefinedPowerModule, RefinedPower);
