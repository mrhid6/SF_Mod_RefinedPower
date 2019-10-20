// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AlpakitCommands.h"

#define LOCTEXT_NAMESPACE "FAlpakitModule"

void FAlpakitCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Alpakit", "Open the alpakit window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
