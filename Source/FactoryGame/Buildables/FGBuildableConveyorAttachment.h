// Copyright 2017 Coffee Stain Studios. All Rights Reserved.

#pragma once
#include "UObject/Class.h"

#include "CoreMinimal.h"
#include "FGBuildableFactory.h"
#include "FGBuildableConveyorAttachment.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYGAME_API AFGBuildableConveyorAttachment : public AFGBuildableFactory
{
	GENERATED_BODY()
public:
	AFGBuildableConveyorAttachment();

	//~ Begin IFGDismantleInterface
	virtual void Dismantle_Implementation() override;
	//~ End IFGDismantleInferface
};
