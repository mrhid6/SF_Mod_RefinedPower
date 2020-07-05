#pragma once

#include "CoreMinimal.h"
#include "Core.h"
#include "Buildables/FGBuildableFactory.h"
#include "FGRemoteCallObject.h"
#include "Components/BoxComponent.h"
#include "RPMPCore.h"
#include "RPMPPlatform.generated.h"


UENUM(BlueprintType)
enum class EMPPlatformType : uint8
{
	MP_BoilerPlatform	    UMETA(DisplayName = "Boiler"),
	MP_ConverterPlatform	UMETA(DisplayName = "Converter"),
	MP_CoolerPlatform		UMETA(DisplayName = "Cooler")
};

UCLASS()
class REFINEDPOWER_API ARPMPPlatform : public AFGBuildableFactory
{
	GENERATED_BODY()
public:
		ARPMPPlatform();
	~ARPMPPlatform();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Factory_Tick(float dt) override;
	virtual void Tick(float dt) override;

	void UpdatePlatformAttachments();

		void SetupInitalPlacement();
		void SetupConnectionToCore(ARPMPCore* MPCore);

		UFUNCTION(NetMulticast, Reliable)
			void Multicast_CoreConnectionUpdated();

		UFUNCTION(BlueprintImplementableEvent, Category = "RefinedPower|ModularPower")
			void OnRep_CoreConnectionUpdated();

		TArray<AActor*> GetAttachedMPBuildings();

	protected:

		UPROPERTY(BlueprintReadOnly, SaveGame, Replicated)
			bool mConnectedToCore;

		bool mConnectionToCoreUpdated;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
			EMPPlatformType mPlatformType;
};