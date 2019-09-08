// Copyright 2016 Coffee Stain Studios. All Rights Reserved.

#pragma once
#include "Array.h"
#include "UnrealString.h"
#include "SubclassOf.h"
#include "UObject/Class.h"

#include "GameFramework/CheatManager.h"
#include "FGGameState.h"
#include "Interfaces/OnlineSharedCloudInterface.h"
#include "FGCheatManager.generated.h"

/**
 * 
 */
UCLASS( Within = PlayerController, Config = Editor )
class FACTORYGAME_API UFGCheatManager : public UCheatManager
{
	GENERATED_BODY()
public:
	virtual void InitCheatManager() override;

	UFUNCTION( exec)
	virtual void NoCost( bool enabled );

	UFUNCTION( exec)
	virtual void NoPower( bool enabled );

	UFUNCTION( exec)
	virtual void TurboMode( bool enabled );

	UFUNCTION( exec)
	virtual void GiveItem( TSubclassOf< class UFGItemDescriptor > resource, int32 amount );

	UFUNCTION( exec )
	virtual void ClearGiveItemPopularList();
	
	UFUNCTION( exec)
	virtual void GiefALLSchematics();

	UFUNCTION( exec)
	void GiefAllSchematicsAndPhases();

	UFUNCTION( exec)
	virtual void GiefAvailableSchematics();

	UFUNCTION( exec)
	virtual void GiefCheatSchematics();

	UFUNCTION( exec)
	virtual void GiefStorySchematics();

	UFUNCTION( exec)
	void GiefStartingResearch();
	
	UFUNCTION( exec )
	virtual void RebuildPowerCircuits();
	
	UFUNCTION( exec )
	void EnableBuildableTick( bool enable );

	/** Cheat for disabling the players fov override if we want to get the fov from cameras in world */
	UFUNCTION( exec )
	void EnablePlayerFOV( bool enable );
	
	UFUNCTION( exec )
	void DestroyPawn();

	UFUNCTION( exec )
	void RemoveAllFoliage( int32 maxNumInstances = 999999 );

	UFUNCTION( exec)
	virtual void PardonAllPlayers();

	UFUNCTION( exec)
	virtual void ClearPardon();

	UFUNCTION( exec)
	void SetTimeOfDay( int32 hour, int32 minute = 0 );

	/** Hide everything except the factory */
	UFUNCTION( exec )
	void ShowFactoryOnly( bool environmentHidden );

	/** Hide everything the factory */
	UFUNCTION( exec )
	void HideFactoryOnly( bool factoryHidden );

	UFUNCTION( exec )
	void DumpAllAvailableRecipes();

	UFUNCTION( exec )
	void ToggleAudioDebug();

	UFUNCTION( exec)
	void SetSlomo( float slomo );

	UFUNCTION( exec )
	void OpenModMap();

	UFUNCTION( exec)
	void PumpiMode( bool enable );

	UFUNCTION( exec )
	void SplitAllConveyors();

	UFUNCTION( exec )
	void MergeAllConveyors();

	UFUNCTION( exec) 
	void SetTimeSpeedMultiplier( float speed );

	UFUNCTION( exec )
	void SetFactoryDetailReplication( bool enable );

	UFUNCTION( exec )
	void ResetFuses();

	UFUNCTION( exec)
	void ToggleCameraMode();

	UFUNCTION( exec)
	void DumpFactoryStatsToLog();

	UFUNCTION( exec)
	void GiefSchematicsOfTier( int32 tier );

	UFUNCTION( exec)
	void SetGamePhase( EGamePhase phase );

	UFUNCTION( exec )
	void TestSharedInventoryPtr();

	/** Forces active spawners to spawn creatures even if the creature isn't set to spawn yet ( because of day/night restrictions etc ) */
	UFUNCTION( exec )
	void ForceSpawnCreatures();

	UFUNCTION( exec )
	void DumpNonDormantActors();

	UFUNCTION( exec)
	void ToggleAudioLandingDebug();

	UFUNCTION( exec )
	void DumpAttachedToSkelMesh( bool detailed = false );

	UFUNCTION( exec )
	void DumpTicking( bool detailed = false );
	
	/** Dump all GPU particles systems that's available */
	UFUNCTION( exec )
	void DumpActiveGPUParticles();

	/** Starts replaying buildeffects */
	UFUNCTION( exec )
	void ReplayBuildingEffects();

	/** Sets visibility on all buildings */
	UFUNCTION( exec )
	void HideAllBuildings( bool inVisibility );

	/** Randomize building color slot color */
	UFUNCTION( exec )
	void RandomizeBuildingsColorSlot( uint8 slotIndex = 0);

	UFUNCTION( exec )
	void UpdateSessionToOSS();

	UFUNCTION( exec )
	void VisitAllMapAreas();

	UFUNCTION( exec ) 
	void SetAITickDistance( float distance );

	UFUNCTION( exec )
	void DumpPlayerStates();

	UFUNCTION( exec )
	void EnableInstancingOnFactory( bool enabled );

	UFUNCTION( exec )
	void DumpActorRepresentations();

	UFUNCTION( exec )
	void PurgeInactiveClientsFromSave( bool fetchInventories );

	UFUNCTION( exec )
	void PurgeAllBeaconsFromSave();

	UFUNCTION( exec )
	void ListItemPickups();

	UFUNCTION( exec ) 
	void SetTradingPostLevel( int32 inLevel );

	UFUNCTION( exec )
	void ListUnlockedRecipesAndSchematics();

	UFUNCTION( exec )
	void SaveWithNewSessionName( const FString& saveName, const FString& sessionName );

	UFUNCTION( exec ) 
	void GetVehicleInfo();

	UFUNCTION( exec )
	void RunDebugMetric();

	UFUNCTION( exec )
	void MCP_Login( FString username, FString password );

	UFUNCTION( exec )
	void MCP_Logout();

	UFUNCTION( exec )
	void MCP_GetOnlineStatus( int32 localPlayerNum = 0 );

	UFUNCTION( exec )
	void MCP_UpdatePresence( FString key, FString value );

	UFUNCTION( exec )
	void MCP_UpdatePresenceString( FString string );

	UFUNCTION( exec )
	void MCP_LogPresence();

	UFUNCTION( exec )
	void MCP_GetFriends();

	UFUNCTION( exec )
	void TriggerPresenceUpdate();

	UFUNCTION( exec )
	void ShowSequenceList();

	UFUNCTION( exec )
	void SendInviteToFriend( FString friendName );

	UFUNCTION( exec )
	void DebugCrash();

	UFUNCTION( exec )
	void ResetHubTutorial();

	UFUNCTION( exec )
	void ResetSchematics();

	UFUNCTION( exec )
	void DumpSchematics();

	UFUNCTION( exec )
	void FixupBuiltByRecipeInOldSave( bool reapplyRecipeIfBetterMatchFound = false );

	UFUNCTION( exec )
	void PrintStatichMeshesHirarchy();

	UFUNCTION( exec )
	void FlipVehicle();

	UFUNCTION( exec )
	void ListDebugStartingPoint();
	
	UFUNCTION( exec )
	void SetDebugStartingPoint( FName startingPoint );
	
	UFUNCTION( exec )
	void SetRandomDebugStartingPoint();

	UFUNCTION( exec )
	void CompleteResearch();

	UFUNCTION( exec )
	void PurgeAllTrainState();

public:
	/** This is used to make picking the same classes in the cheat board easier */
	UPROPERTY( Config )
	TArray< UClass* > mPopularUClassChoices;
};
