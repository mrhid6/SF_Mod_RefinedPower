// ILikeBanas

#include "Turbine/RPWaterTurbineNode.h"

// Sets default values
ARPWaterTurbineNode::ARPWaterTurbineNode()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    mCanPlacePortableMiner = false;

    mPurity = EResourcePurity::RP_Pure;
    mAmount = EResourceAmount::RA_Infinite;

    SetReplicates(true);
}

// Called when the game starts or when spawned
void ARPWaterTurbineNode::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ARPWaterTurbineNode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARPWaterTurbineNode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARPWaterTurbineNode, mHasTurbine)
}


FText ARPWaterTurbineNode::GetLookAtDecription_Implementation(class AFGCharacterPlayer* byCharacter,
                                                              const FUseState& state) const
{
    int32 outputPower = (int32)GetTypePowerProduction();

    FString resString = FString("Water Turbine Node [");
    resString.Append(FString::FromInt(outputPower));
    resString.Append(" MW]");

    return FText::FromString(resString);
}

float ARPWaterTurbineNode::GetTypePowerProduction() const
{
    if (mNodeType == EWaterTurbineNodeType::RP_SLOW)
    {
        return 8.0f;
    }
    else if (mNodeType == EWaterTurbineNodeType::RP_MEDIUM)
    {
        return 20.0f;
    }
    else
    {
        return 50.0f;
    }
}

bool ARPWaterTurbineNode::CanPlaceResourceExtractor() const
{
    bool IDontCare = Super::CanPlaceResourceExtractor();
    return false;
}

void ARPWaterTurbineNode::OnUse_Implementation(class AFGCharacterPlayer* byCharacter, const FUseState& state)
{
}

void ARPWaterTurbineNode::OnHasTurbineChanged_Implementation()
{
};

void ARPWaterTurbineNode::ExtractResourceAndGiveToPlayer(AFGCharacterPlayer* toPlayer, int32 amount)
{
}
