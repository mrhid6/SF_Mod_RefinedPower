// ILikeBanas

#include "RefinedPower.h"
#include "FGGameMode.h"
#include "DysonSphere/RPDysonSphere.h"
#include "ModularPower/Buildings/RPMPTurbineBuilding.h"
#include "Patching/NativeHookManager.h"
#include "Reactor/RPArcReactor.h"
#include "Reactor/RPMicroReactor.h"
#include "SolarPanel/RPSolarPanel.h"
#include "Turbine/RPTurbineBaseActor.h"

#define LOCTEXT_NAMESPACE "FRefinedPowerModule"

void GameModePostLogin(CallScope<void(*)(AFGGameMode*, APlayerController*)>& scope, AFGGameMode* gm,
                       APlayerController* pc)
{
	if (gm->HasAuthority() && !gm->IsMainMenuGameMode())
	{
		//SML::Logging::info("[RefinedPower] - Register RCO");
		gm->RegisterRemoteCallObjectClass(URPTurbineBaseRCO::StaticClass());
		gm->RegisterRemoteCallObjectClass(URPArcReactorRCO::StaticClass());
		gm->RegisterRemoteCallObjectClass(URPMicroReactorRCO::StaticClass());
		gm->RegisterRemoteCallObjectClass(URPDysonSphereRCO::StaticClass());
		gm->RegisterRemoteCallObjectClass(URPSolarPanelRCO::StaticClass());
		gm->RegisterRemoteCallObjectClass(URPMPBuildingRCO::StaticClass());
		gm->RegisterRemoteCallObjectClass(URPMPTurbineBuildingRCO::StaticClass());
	}
}

void FRefinedPowerModule::StartupModule()
{
#if !WITH_EDITOR
	// Hooking to register RCOs
	AFGGameMode* LocalGameMode = GetMutableDefault<AFGGameMode>();
	SUBSCRIBE_METHOD_VIRTUAL(AFGGameMode::PostLogin, LocalGameMode, &GameModePostLogin)
#endif
}

void FRefinedPowerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Nothing here... yet
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRefinedPowerModule, RefinedPower)