#pragma once

#include "CoreMinimal.h"
#include "Core.h"
#include "Buildables/FGBuildableGenerator.h"
#include "FGRemoteCallObject.h"
#include "Components/BoxComponent.h"
#include "RPMPPlacementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPMPPlatform.generated.h"


UENUM(BlueprintType)
enum class EMPPlatformType : uint8
{
    MP_BoilerPlatform UMETA(DisplayName = "Boiler"),
    MP_ConverterPlatform UMETA(DisplayName = "Converter"),
    MP_CoolerPlatform UMETA(DisplayName = "Cooler")
};

UCLASS()
class REFINEDPOWER_API ARPMPPlatform : public AFGBuildableGenerator
{
    GENERATED_BODY()
public:
    ARPMPPlatform();
    ~ARPMPPlatform();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void BeginPlay() override;
    virtual void Factory_Tick(float dt) override;
    virtual void Tick(float dt) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void UpdatePlatformAttachments();

    void AttachBuildingToComp(AActor* Actor);
    void DetachBuildingFromComp(AActor* Actor);

    URPMPPlacementComponent* GetPlacementComponent(EMPPlatformBuildingType type);

    TArray<AActor*> GetAttachedMPBuildings();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
    EMPPlatformType mPlatformType;
};
