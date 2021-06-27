// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Resources/FGResourceNode.h"

#include "RPWaterTurbineNode.generated.h"

UENUM(BlueprintType)
enum class EWaterTurbineNodeType : uint8
{
    RP_SLOW UMETA(DisplayName = "Slow"),
    RP_MEDIUM UMETA(DisplayName = "Medium"),
    RP_FAST UMETA(DisplayName = "Fast")
};

UCLASS(Blueprintable)
class REFINEDPOWER_API ARPWaterTurbineNode : public AFGResourceNode
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ARPWaterTurbineNode();

    virtual FText GetLookAtDecription_Implementation(class AFGCharacterPlayer* byCharacter,
                                                     const FUseState& state) const override;

    virtual bool CanPlaceResourceExtractor() const override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnHasTurbineChanged, SaveGame, Category =
        "Refined Power")
    bool mHasTurbine = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Refined Power")
    EWaterTurbineNodeType mNodeType = EWaterTurbineNodeType::RP_SLOW;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

    UFUNCTION(BlueprintPure, Category = "Refined Power|Turbine|Node")
    float GetTypePowerProduction() const;

    UFUNCTION(BlueprintNativeEvent)
    void OnHasTurbineChanged();

    virtual void ExtractResourceAndGiveToPlayer(AFGCharacterPlayer* toPlayer, int32 amount = 1) override;

    virtual void OnUse_Implementation(class AFGCharacterPlayer* byCharacter, const FUseState& state) override;
};
