#pragma once

#include "CoreMinimal.h"
#include "Core.h"
#include "Buildables/FGBuildableGenerator.h"
#include "FGRemoteCallObject.h"
#include "Components/BoxComponent.h"
#include "RPMPCore.h"
#include "RPMPBuilding.generated.h"


UCLASS()
class REFINEDPOWER_API ARPMPBuilding : public AFGBuildableGenerator
{
	GENERATED_BODY()

		ARPMPBuilding();
	~ARPMPBuilding();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Factory_Tick(float dt) override;
	virtual void Tick(float dt) override;

	public:
		void SetupInitalPlacement();
		void SetupConnectionToCore(ARPMPCore* MPCore);

		UFUNCTION(NetMulticast, Reliable)
			void Multicast_CoreConnectionUpdated();

		UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|ModularPower")
			void OnRep_CoreConnectionUpdated();

	protected:

		UPROPERTY(BlueprintReadOnly, SaveGame, Replicated)
			bool mConnectedToCore;

		bool mConnectionToCoreUpdated;
};