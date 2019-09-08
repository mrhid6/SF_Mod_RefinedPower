#pragma once
#include "Engine/StaticMesh.h"
#include "Array.h"
#include "UnrealString.h"
#include "GameFramework/Actor.h"
#include "UObject/Class.h"

#include "Components/SceneComponent.h"
#include "FGSaveInterface.h"
#include "DefaultValueHelper.h"
#include "FGFactoryLegsComponent.generated.h"

USTRUCT()
struct FFeetOffset
{
	GENERATED_BODY()

	FFeetOffset(){}

	FFeetOffset( const uint8 inFeetIndex ) :
		FeetIndex( inFeetIndex ),
		OffsetZ( 0.f ),
		ShouldShow( true ),
		IsValidOffset( true )
	{
	}

	/** The name of the foot's socket. */
	UPROPERTY( SaveGame )
	uint8 FeetIndex;

	/** The offset along the Z axis from the parent mesh origo. */
	UPROPERTY( SaveGame )
	float OffsetZ;

	//@todo Not used anymore.
	/** The foot should potentially be invisible. */
	UPROPERTY( SaveGame )
	uint8 ShouldShow : 1;

	/** Does this foot have a valid offset. */
	UPROPERTY( )
	uint8 IsValidOffset : 1;

	FName GetSocketNameFromIndex() const
	{
		TArray<FStringFormatArg> args;
		args.Add( FString::FromInt( FeetIndex ) );
		const FName socketName( *FString::Format( TEXT( "foot_0{0}" ), args ) );
		return socketName;
	}

	static uint8 GetFeetIndexFromSocket( const FName& socketName )
	{
		FString socketIndexString = socketName.ToString( );
		socketIndexString.ReplaceInline( TEXT("foot_"), TEXT(""), ESearchCase::IgnoreCase );

		int32 value;
		if( FDefaultValueHelper::ParseInt( socketIndexString, value ) )
		{
			return uint8( value );
		}

		return 0;
	}
};

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class UFGFactoryLegsComponent : public USceneComponent, public IFGSaveInterface
{
	GENERATED_BODY()
public:
	// Begin AActor interface
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;


	UFGFactoryLegsComponent();

	//~ Begin UActorComponent interface
#if WITH_EDITOR
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
#endif
	virtual void BeginPlay() override;
	virtual void EndPlay( const EEndPlayReason::Type endPlayReason ) override;
	//~ End UActorComponent interface

	// Begin IFGSaveInterface
	virtual void PreSaveGame_Implementation( int32 saveVersion, int32 gameVersion ) override;
	virtual void PostSaveGame_Implementation( int32 saveVersion, int32 gameVersion ) override;
	virtual void PreLoadGame_Implementation( int32 saveVersion, int32 gameVersion ) override;
	virtual void PostLoadGame_Implementation( int32 saveVersion, int32 gameVersion ) override;
	virtual void GatherDependencies_Implementation( TArray< UObject* >& out_dependentObjects ) override;
	virtual bool NeedTransform_Implementation() override;
	virtual bool ShouldSave_Implementation() const override;
	// End IFSaveInterface

	/** Get all the leg meshes. */
	UFUNCTION( BlueprintCallable, Category = "Legs" )
	TArray< class UStaticMeshComponent* > GetLegMeshComponents() const { return mLegMeshComponents; }
	/** Get all the foot meshes. */
	UFUNCTION( BlueprintCallable, Category = "Legs" )
	TArray< class UStaticMeshComponent* > GetFootMeshComponents() const { return mFootMeshComponents; }

	/** Trace for the feet offsets and return the results. */
	TArray< FFeetOffset > TraceFeetOffsets( const FTransform& actorTransform ) const;

	/** Set/get the feet offsets from external data, must be set prior to BeginPlay. */
	void SetFeetOffsets( const TArray< FFeetOffset >& offsets );
	TArray< FFeetOffset > GetCachedFeetOffsets() const { return mCachedFeetOffset; }

	float GetMaxLegLength() const;

	static bool GetTracedFeetOffsetFromGround( const FTransform& actorTransform, UFGFactoryLegsComponent* legsComp, TArray<FFeetOffset>& out_tracedFeetOffsets, float& out_highestFootOffset, float& out_longestLeg );

private:
	FFeetOffset TraceFootOffset( FName socket, const FTransform& actorTransform ) const;

	class UStaticMesh* GetLegMesh() const;
	class UStaticMesh* GetFootMesh() const;

	void CreateLegs();
	void RemoveLegs();

	void CreateFoot( const FFeetOffset& offset );

	bool CachedFeetOffsetsAreValid();

protected:
	/** Socket names on the parent mesh */
	UPROPERTY( EditDefaultsOnly, Category = "Legs" )
	TArray< FName > mLegSocketNames;

	/** The mesh to be used as legs on factories */
	UPROPERTY( EditDefaultsOnly, Category = "Legs" )
	class UStaticMesh* mLegMeshOverride;

	/** The mesh to be used as foots on factories */
	UPROPERTY( EditDefaultsOnly, Category = "Legs" )
	class UStaticMesh* mFootMeshOverride;

	/** The maximum length the legs can be. */
	UPROPERTY( EditDefaultsOnly, Category = "Legs" )
	float mMaxLegLengthOverride;

private:
	/** The created leg components for this building */
	UPROPERTY( transient )
	TArray< UStaticMeshComponent* > mLegMeshComponents;

	/** The created foot components for this building */
	UPROPERTY( transient )
	TArray< UStaticMeshComponent* > mFootMeshComponents;

	/** Stored so that we know the offset of the feet */
	UPROPERTY( SaveGame, Replicated )
	TArray< FFeetOffset > mCachedFeetOffset;
};
