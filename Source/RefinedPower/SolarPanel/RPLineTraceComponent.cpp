#include "RPLineTraceComponent.h"

URPLineTraceComponent::URPLineTraceComponent()
{
    mLineLength = 0;
}

URPLineTraceComponent::~URPLineTraceComponent()
{
}

void URPLineTraceComponent::TraceLine(FHitResult& OutHit)
{
    FVector ForwardVector = GetForwardVector();
    FVector CompLocation = GetComponentLocation();

    FVector Start = (CompLocation);

    FVector End = ((ForwardVector * mLineLength) + Start);
    FCollisionQueryParams CollisionParams;

    GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);
};
