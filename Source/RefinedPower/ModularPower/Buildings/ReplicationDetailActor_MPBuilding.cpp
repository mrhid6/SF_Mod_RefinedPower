#include "ReplicationDetailActor_MPBuilding.h"
#include "UnrealNetwork.h"
#include "../RPMPBuilding.h"
#include "FGInventoryLibrary.h"

//Add our fields to replicated list
void AReplicationDetailActor_MPBuilding::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AReplicationDetailActor_MPBuilding, MPInventoryComponent);
}

//Create our inventory components, copy state from original, set replicated inventory components
void AReplicationDetailActor_MPBuilding::InitReplicationDetailActor(AFGBuildable* owningActor)
{
    Super::InitReplicationDetailActor(owningActor);
    //Remember: UPROPERTY(Replicated) will only replicate property value, not the actual component
    MPInventoryComponent = UFGInventoryLibrary::CreateInventoryComponent(this, TEXT("MPInventory"));
    MPInventoryComponent->SetIsReplicated(true);

    //Copy component states from the owner building
    ARPMPBuilding* MPBuilding = Cast<ARPMPBuilding>(owningActor);
    MPInventoryComponent->CopyFromOtherComponent(MPBuilding->MPInventoryComponent);

    //Set replicated component values for host. For client they are set in OnRep_ReplicationDetailActor
    //After that, WE are considered the MASTER state of the inventories
    //So inventory WE own is actually what represents REAL STATE
    MPBuilding->MPInventoryHandler->SetReplicationInventoryComponent(MPInventoryComponent);
}

//Copy replicated component values back to main inventory components
void AReplicationDetailActor_MPBuilding::FlushReplicationActorStateToOwner()
{
    Super::FlushReplicationActorStateToOwner();
    if (mOwningBuildable)
    {
        //Copy replicated component values back to buildable inventory components
        //After that, WE NO LONGER represent a MASTER state of the inventories
        //REAL STATE is owned by the BUILDABLE inventories themselves now
        ARPMPBuilding* MPBuilding = Cast<ARPMPBuilding>(mOwningBuildable);
        MPBuilding->MPInventoryComponent->CopyFromOtherComponent(MPInventoryComponent);
    }
}

bool AReplicationDetailActor_MPBuilding::HasCompletedInitialReplication() const
{
    return Super::HasCompletedInitialReplication() && MPInventoryComponent;
}
