#pragma once
#include "FGReplicationDetailActor_BuildableFactory.h"
#include "ReplicationDetailActor_MPBuilding.generated.h"

UCLASS()
class AReplicationDetailActor_MPBuilding : public AFGReplicationDetailActor_BuildableFactory
{
    GENERATED_BODY()
public:
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    void InitReplicationDetailActor(AFGBuildable* owningActor) override;
    void FlushReplicationActorStateToOwner() override;
    bool HasCompletedInitialReplication() const override;
public:
    friend class AMPBuilding;
    UPROPERTY(Replicated)
    UFGInventoryComponent* MPInventoryComponent;
};
