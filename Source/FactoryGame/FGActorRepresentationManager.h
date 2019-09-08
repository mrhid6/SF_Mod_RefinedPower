// Copyright 2016 Coffee Stain Studios. All Rights Reserved.

#pragma once
#include "UObject/CoreNet.h"
#include "Engine/World.h"
#include "Array.h"
#include "GameFramework/Actor.h"
#include "UObject/Class.h"

#include "FGSubsystem.h"
#include "FGActorRepresentation.h"
#include "FGActorRepresentationManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnActorRepresentationAdded, UFGActorRepresentation*, newRepresentation );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnActorRepresentationRemoved, UFGActorRepresentation*, removedRepresentation );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnActorRepresentationUpdated, UFGActorRepresentation*, updatedRepresentation );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FOnActorRepresentationUpdatedCompassShow, UFGActorRepresentation*, updatedRepresentation, bool, newShouldShowInCompass );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FOnActorRepresentationUpdatedMapShow, UFGActorRepresentation*, updatedRepresentation, bool, newShouldShowOnMap );

/**
 * This class manages all the representations of actors that are used in the compass and on the map.
 */
UCLASS()
class FACTORYGAME_API AFGActorRepresentationManager : public AFGSubsystem
{
	GENERATED_BODY()
public:
	/** Get the representation manager, this should always return something unless you call it really early. */
	static AFGActorRepresentationManager* Get( UWorld* world );

	/** Get the representation manager from a world context, this should always return something unless you call it really early. */
	UFUNCTION( BlueprintPure, Category = "Representation", DisplayName = "GetActorRepresentationManager", Meta = ( DefaultToSelf = "worldContext" ) )
	static AFGActorRepresentationManager* Get( UObject* worldContext );

public:
	AFGActorRepresentationManager();

	/** Replication */
	void GetLifetimeReplicatedProps( TArray< FLifetimeProperty >& OutLifetimeProps ) const override;
	virtual void PreReplication( IRepChangedPropertyTracker& changedPropertyTracker ) override;
	virtual bool ReplicateSubobjects( class UActorChannel* channel, class FOutBunch* bunch, FReplicationFlags* repFlags ) override;

	/** Creates a new actor representation. Must be called on server for representation that aren't local to appear for all players. */
	UFUNCTION( BlueprintCallable, Category = "Representation" )
	bool CreateAndAddNewRepresentation( AActor* realActor, bool isLocal = false );

	UFUNCTION( BlueprintCallable, Category = "Representation" )
	bool UpdateRepresentation( AActor* realActor, bool isLocal = false );

	/** Creates a new representation for something that doesn't have an actor. Must be called on server for representations that aren't local to appear for all players. */
	UFUNCTION( BlueprintCallable, Category = "Representation" )
	bool CreateAndAddNewRepresentationNoActor( FVector location, class UTexture2D* compassTexture, FLinearColor compassColor, float lifeTime, bool shouldShowInCompass, bool shouldShowOnMap, ERepresentationType representationType = ERepresentationType::RT_Default, bool isLocal = true );

	/** Removes the representation of an actor */
	UFUNCTION( BlueprintCallable, BlueprintAuthorityOnly, Category = "Representation" )
	bool RemoveRepresentationOfActor( AActor* realActor );

	/** Gathers all representations and returns them */
	UFUNCTION( BlueprintCallable, Category = "Representation" )
	void GetAllActorRepresentations( UPARAM( ref ) TArray< class UFGActorRepresentation* >& out_AllRepresentations );
	
	/** Debug */
	void DumpActorRepresentations();

protected:
	// Begin AActor interface
	virtual void Tick( float dt ) override;
	// End AActor interface

private:
	UFUNCTION()
	void OnRep_ReplicatedRepresentations();

public:
	/** Called whenever a new representation is added */
	UPROPERTY( BlueprintAssignable, Category = "Representation" )
	FOnActorRepresentationAdded mOnActorRepresentationAdded;

	/** Called whenever a new representation is removed, not sure how long it will be valid after a remove. */
	UPROPERTY( BlueprintAssignable, Category = "Representation" )
	FOnActorRepresentationRemoved mOnActorRepresentationRemoved;

	/** Called whenever a representation is updated */
	UPROPERTY( BlueprintAssignable, Category = "Representation" )
	FOnActorRepresentationUpdated mOnActorRepresentationUpdated;

	/** Called whenever a new representation changes it's show value for the compass */
	UPROPERTY( BlueprintAssignable, Category = "Representation" )
	FOnActorRepresentationUpdatedCompassShow mOnActorRepresentationUpdatedCompassShow;

	/** Called whenever a new representation changes it's show value for the maps */
	UPROPERTY( BlueprintAssignable, Category = "Representation" )
	FOnActorRepresentationUpdatedMapShow mOnActorRepresentationUpdatedMapShow;

private:
	/** These are all the representations of actors that should replicate from server to clients */
	UPROPERTY( ReplicatedUsing = OnRep_ReplicatedRepresentations )
	TArray< class UFGActorRepresentation* > mReplicatedRepresentations;

	/** Simulated on client to keep track of added and removed representations */
	UPROPERTY()
	TArray< UFGActorRepresentation* > mClientReplicatedRepresentations;

	/** These are representation that the local player adds for them selves, often temporary stuff that others shouldn't see */
	UPROPERTY()
	TArray< UFGActorRepresentation* > mLocalRepresentations;
};
