#include "RPLineTraceComponent.h"

URPLineTraceComponent::URPLineTraceComponent() {
	mLineLength = 0;
}

URPLineTraceComponent::~URPLineTraceComponent() {}

void URPLineTraceComponent::TraceLine(FHitResult &OutHit)
{
	FVector ForwardVector = GetForwardVector();
	FVector CompLocation = GetComponentLocation();

	FVector Start = (CompLocation);

	FVector End = ((ForwardVector * mLineLength) + Start);
	FCollisionQueryParams CollisionParams;

	SML::Logging::info("[RefinedPower] - Tyring WorldStatic");
	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_WorldStatic, CollisionParams);

	if (isHit)
	{
		if (OutHit.bBlockingHit)
		{
			FString ActorName = OutHit.GetActor()->GetName();
			SML::Logging::info(TCHAR_TO_UTF8(*ActorName));
		}
	}
	else {
		SML::Logging::info("[RefinedPower] - Tyring WorldDynamic");
		isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_WorldDynamic, CollisionParams);

		if (isHit)
		{
			if (OutHit.bBlockingHit)
			{
				FString ActorName = OutHit.GetActor()->GetName();
				SML::Logging::info(TCHAR_TO_UTF8(*ActorName));
			}
		}
	}

};