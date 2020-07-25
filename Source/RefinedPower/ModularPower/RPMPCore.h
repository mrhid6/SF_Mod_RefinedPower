#pragma once

#include "CoreMinimal.h"
#include "Core.h"
#include "Buildables/FGBuildableGenerator.h"
#include "FGRemoteCallObject.h"
#include "Components/SphereComponent.h"
#include "RPMPCore.generated.h"


UCLASS()
class REFINEDPOWER_API ARPMPCore : public AFGBuildableGenerator
{
    GENERATED_BODY()

    ARPMPCore();
    ~ARPMPCore();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
    virtual void Factory_Tick(float dt) override;
    virtual void Tick(float dt) override;

public:
    void CacheNearbyMPBuildings();
    void ConfigureNearbyMPBuildings();
    void UnconfigureNearbyMPBuildings();

    void UpdatePowerData();

protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    class USphereComponent* mPowerModuleRange;

    TArray<AActor*> mCachedNearbyMPPlatforms;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UFGPowerConnectionComponent* FGPowerConnection;
};
