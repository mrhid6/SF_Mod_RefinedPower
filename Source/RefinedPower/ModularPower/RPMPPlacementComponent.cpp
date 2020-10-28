// ILikeBanas


#include "RPMPPlacementComponent.h"
#include "RPMPBuilding.h"


// Sets default values for this component's properties
URPMPPlacementComponent::URPMPPlacementComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    bReplicates = true;
    SetIsReplicated(true);

    // ...
}


// Called when the game starts
void URPMPPlacementComponent::BeginPlay()
{
    Super::BeginPlay();
    // ...
}


// Called every frame
void URPMPPlacementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void URPMPPlacementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(URPMPPlacementComponent, mOccupied);
}


bool URPMPPlacementComponent::IsOccupied()
{
    return mOccupied;
}

void URPMPPlacementComponent::OnRep_IsOccupied()
{
}
