#pragma once
#include "Engine/DataTable.h"
#include "Resources/FGResourceNode.h"
#include "FNodeLocations.generated.h"

USTRUCT(BlueprintType)
struct FNodeLocations : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Node")
	TEnumAsByte<EResourcePurity> ResourcePurity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Node")
	FVector Position;
};
