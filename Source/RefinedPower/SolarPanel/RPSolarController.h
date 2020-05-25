// ILikeBanas

#pragma once

#include "CoreMinimal.h"
#include "Core.h"
#include "Engine.h"
#include "FGTimeSubsystem.h"
#include "FGSubsystem.h"
#include "Buildables/FGBuildableFactory.h"
#include "GameFramework/Actor.h"
#include "util/Logging.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "RPSolarController.generated.h"

UCLASS()
class REFINEDPOWER_API ARPSolarController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPSolarController();
	~ARPSolarController();

	// Called when in range (Not needed)
	void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

	// Called when the game starts or when spawned
	void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;


	UFUNCTION(BlueprintCallable)
	static ARPSolarController* Get(UWorld* world);

	/*getters and setters*/
	FRotator GetOrientation();
	float GetCurrectProductionScalar();

	void CacheInstancedMeshPool();

	class UHierarchicalInstancedStaticMeshComponent* mPanelMeshPool;
	class UHierarchicalInstancedStaticMeshComponent* mSupportMeshPool;

	TMap<uint32, uint32> IdToInstanceMapping;
	TArray<uint32> IdBuffer;
	UFUNCTION(BlueprintCallable)
	void SpawnIM(FTransform initPanelTransform, FTransform initSupportTransform, int actorId);
	void DestroyIM(int actorId);
	void FinishUpdates();

	void UpdateSolarPanelsRotation();

protected:
	/*the current production scalar based on the solarProductionScalar function*/
	UPROPERTY(Replicated)
	float mCurrentProductionScalar = 0;

	/*correct rotator for solar panel orientation*/
	UPROPERTY(Replicated)
	FRotator mOrientation = FRotator(0.0,0.0,0.0);

	/*cached sun and moon actors - [0] is moon - [1] is sun*/
	TArray<AActor*> moonsun;

	TArray<AActor*> GetMoonSunActors();

	ADirectionalLight* GetSunActor();
	ADirectionalLight* GetMoonActor();

	FTimerHandle mRotationTimerHandle;
	FTimerHandle mScalarTimerHandle;

private:

	/*cache sun&moon actors*/
	void CacheMoonSunActors();

	void UpdateSolarProductionScalar();
	void UpdateCorrectOrientation();

	/*cached time subsystem to avoid getting every actor tick*/
	AFGTimeOfDaySubsystem* timeSys;

};
