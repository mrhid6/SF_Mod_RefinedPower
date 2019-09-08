#pragma once
#include "UObject/Class.h"
#include "Curves/CurveFloat.h"
#include "Engine/DeveloperSettings.h"
#include "FGDeveloperSettings.generated.h"

UCLASS( config = Game, defaultconfig, meta = ( DisplayName = "Satisfactory Developer Settings" ) )
class FACTORYGAME_API UFGDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY( EditAnywhere, config, Category = Focus, meta = ( ToolTip = "Should widgets grab focus back on focus lost in PIE" ) )
	bool mGreedyFocusInPIE;

	UPROPERTY( EditAnywhere, config, Category = Map, meta = ( ToolTip = "" ) )
	float mLowestWorldLocation;

	UPROPERTY( EditAnywhere, config, Category = Map, meta = ( ToolTip = "" ) )
	float mHighestWorldLocation;

	UPROPERTY( EditAnywhere, config, Category = FogOfWar, meta = ( ToolTip = "", ClampMin = "0.0", ClampMax = "255.0", UIMin = "0", UIMax = "255.0" ) )
	float mFogOfWarUpperRangeMin;

	UPROPERTY( EditAnywhere, config, Category = FogOfWar, meta = ( ToolTip = "", ClampMin = "0.0", ClampMax = "255.0", UIMin = "0", UIMax = "255.0" ) )
	float mFogOfWarUpperRangeMax;
	
	UPROPERTY( EditAnywhere, config, Category = FogOfWar, meta = ( ToolTip = "", ClampMin = "1.0", ClampMax = "5.0", UIMin = "1.0", UIMax = "5.0" ) )
	float mFogOfWarGradientExpandValue;

	UPROPERTY( EditAnywhere, config, Category = FogOfWar, meta = ( ToolTip = "Enables the fog of war reveal calculations" ) )
	bool mEnableFogOfWarRevealCalculations;

	UPROPERTY( EditAnywhere, config, Category = FogOfWar, meta = ( ToolTip = "Enables the fog of war texture update" ) )
	bool mEnableFogOfWarTextureUpdates;

	UPROPERTY( EditAnywhere, config, Category = FogOfWar, meta = ( ToolTip = "" ) )
	bool mForceSingleThreadedCalculations;

	UPROPERTY( EditAnywhere, config, Category = FogOfWar, meta = ( 
				DisplayName = "Fog of war gradient curve",
				XAxisName = "RadiusScale",
				YAxisName = "GradientScale",
				ToolTip = "Curve that determines the fog of war gradient" ) )
	FRuntimeFloatCurve mFogOfWarGradientCurve;

};