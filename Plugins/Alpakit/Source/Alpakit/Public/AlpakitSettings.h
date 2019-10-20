#pragma once

#include "CoreMinimal.h"
#include "AlpakitSettings.generated.h"

/**
 * Setting object used to hold both config settings and editable ones in one place
 * To ensure the settings are saved to the specified config file make sure to add
 * props using the globalconfig or config meta.
 */
UCLASS(config = Game, defaultconfig)
class UAlpakitSettings : public UObject
{
	GENERATED_BODY()

public:
	UAlpakitSettings(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, config, Category = Custom)
	FText SatisfactoryGamePath;
	
	UPROPERTY(EditAnywhere, config, Category = Custom)
	TArray<FString> Mods;
	
	UPROPERTY(EditAnywhere, config, Category = Custom)
	bool StartGame;

};