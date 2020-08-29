// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Hologram/FGFactoryHologram.h"
#include "Components/BoxComponent.h"
#include "FGWaterVolume.h"
#include "RPDSHologram.generated.h"

/**
 * 
 */
UCLASS()
class REFINEDPOWER_API ARPDSHologram : public AFGFactoryHologram
{
    GENERATED_BODY()

    ARPDSHologram();
    ~ARPDSHologram();
public:
    virtual void BeginPlay() override;
    virtual void CheckValidPlacement() override;

    TArray<AActor*> mCachedDSArr;

protected:

    bool mFoundInWorld;
};
