// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RPMPPlacementComponent.generated.h"


UENUM(BlueprintType)
enum class EMPPlatformBuildingType : uint8
{
	MP_Boiler	    UMETA(DisplayName = "Boiler"),
	MP_Turbine	UMETA(DisplayName = "Turbine"),
	MP_Generator	UMETA(DisplayName = "Generator"),
	MP_Cooler	UMETA(DisplayName = "Cooler")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REFINEDPOWER_API URPMPPlacementComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPMPPlacementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RefinedPower")
		EMPPlatformBuildingType mBuildingType;
};
