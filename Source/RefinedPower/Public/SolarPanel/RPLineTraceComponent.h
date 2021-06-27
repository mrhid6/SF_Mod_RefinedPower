#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "Engine.h"
#include "Components/SceneComponent.h"
#include "RPLineTraceComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class REFINEDPOWER_API URPLineTraceComponent : public USceneComponent
{
    GENERATED_BODY()

    URPLineTraceComponent();
    ~URPLineTraceComponent();

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower", meta = (ClampMin = "0.0", ClampMax =
        "20000.0"))
    float mLineLength;

    void TraceLine(FHitResult& OutHit);
};
