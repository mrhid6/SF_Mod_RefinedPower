// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Alpakit.h"
#include "AlpakitStyle.h"
#include "AlpakitCommands.h"
#include "AlpakitWidget.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "AlpakitSettings.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"

static const FName AlpakitTabName("Alpakit");

#define LOCTEXT_NAMESPACE "FAlpakitModule"

void FAlpakitModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	RegisterSettings();
	
	FAlpakitStyle::Initialize();
	FAlpakitStyle::ReloadTextures();

	FAlpakitCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FAlpakitCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FAlpakitModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("FileProject", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FAlpakitModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FAlpakitModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(AlpakitTabName, FOnSpawnTab::CreateRaw(this, &FAlpakitModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FAlpakitTabTitle", "Alpakit"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FAlpakitModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	if (UObjectInitialized())
	{
		UnregisterSettings();
	}

	FAlpakitStyle::Shutdown();

	FAlpakitCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(AlpakitTabName);
}

TSharedRef<SDockTab> FAlpakitModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SAlpakaWidget)
		];
}


void FAlpakitModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(AlpakitTabName);
}

void FAlpakitModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FAlpakitCommands::Get().OpenPluginWindow);
}

void FAlpakitModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FAlpakitCommands::Get().OpenPluginWindow);
}


// Callback for when the settings were saved.
bool FAlpakitModule::HandleSettingsSaved()
{
	UAlpakitSettings* Settings = GetMutableDefault<UAlpakitSettings>();
	bool ResaveSettings = false;

	// You can put any validation code in here and resave the settings in case an invalid
	// value has been entered

	if (ResaveSettings)
	{
		Settings->SaveConfig();
	}

	return true;
}

void FAlpakitModule::RegisterSettings()
{
	// Registering some settings is just a matter of exposing the default UObject of
	// your desired class, feel free to add here all those settings you want to expose
	// to your LDs or artists.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		UE_LOG(LogTemp, Log, TEXT("Register Settings"));
		// Create the new category
		ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");
		SettingsContainer->DescribeCategory("Alpakit Settings",
			LOCTEXT("RuntimeWDCategoryName", "Alpakit Settings"),
			LOCTEXT("RuntimeWDCategoryDescription", "Alpakit Settings. Can also be edited in the Alpakit menu"));

		// Register the settings
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Alpakit", "General",
			LOCTEXT("RuntimeGeneralSettingsName", "General"),
			LOCTEXT("RuntimeGeneralSettingsDescription", "Alpakit Settings. Can also be edited in the Alpakit menu"),
			GetMutableDefault<UAlpakitSettings>()
		);

		// Register the save handler to your settings, you might want to use it to
		// validate those or just act to settings changes.
		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FAlpakitModule::HandleSettingsSaved);
		}
	}
}

void FAlpakitModule::UnregisterSettings()
{
	// Ensure to unregister all of your registered settings here, hot-reload would
	// otherwise yield unexpected results.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		UE_LOG(LogTemp, Log, TEXT("Unregister Settings"));
		SettingsModule->UnregisterSettings("Project", "Alpakit", "General");
	}
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAlpakitModule, Alpakit)