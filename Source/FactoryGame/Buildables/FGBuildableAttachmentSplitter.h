// Copyright 2017 Coffee Stain Studios. All Rights Reserved.

#pragma once
#include "Array.h"
#include "GameFramework/Actor.h"
#include "SubclassOf.h"
#include "UObject/Class.h"

#include "CoreMinimal.h"
#include "FGBuildableConveyorAttachment.h"
#include "FGBuildableAttachmentSplitter.generated.h"

/**
 * This is a struct that will assign an item to a certain connection.
 */
USTRUCT()
struct FConnectionItemStruct
{
	GENERATED_BODY()

	FConnectionItemStruct() :
		Connection( nullptr ),
		Item( FInventoryItem() ),
		OffsetBeyond( 0.0f )
	{
	}

	FConnectionItemStruct( UFGFactoryConnectionComponent* inConnection, FInventoryItem inItem, float inOffsetBeyond ) :
		Connection( inConnection ),
		Item( inItem ),
		OffsetBeyond( inOffsetBeyond )
	{
	}

	/** The connection to put the item on */
	UPROPERTY()
	class UFGFactoryConnectionComponent* Connection;

	/** The item to put on the connection */
	UPROPERTY()
	FInventoryItem Item;

	/** How far past the conveyor belt length this item is */
	UPROPERTY()
	float OffsetBeyond;
};

/**
 * 
 */
UCLASS()
class FACTORYGAME_API AFGBuildableAttachmentSplitter : public AFGBuildableConveyorAttachment
{
	GENERATED_BODY()
public:
	/** Ctor */
	AFGBuildableAttachmentSplitter();

	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

	// Begin AFGBuildable interface
	virtual void Upgrade_Implementation( AActor* newActor ) override;
	// End AFGBuildable interface

	//~ Begin IFGDismantleInterface
	virtual void GetDismantleRefund_Implementation( TArray< FInventoryStack >& out_refund ) const override;
	// End IFGDismantleInterface

protected:
	// Begin Factory_ interface
	virtual bool Factory_GrabOutput_Implementation( UFGFactoryConnectionComponent* connection, FInventoryItem& out_item, float& out_OffsetBeyond, TSubclassOf< UFGItemDescriptor > type ) override;
	// End Factory_ interface

private:

	void FillDistributionTable();

protected:
	/** Cached input connections for faster lookup. */
	TArray< UFGFactoryConnectionComponent* > mInputs;

	/** Cached output connections for faster lookup. */
	TArray< UFGFactoryConnectionComponent* > mOutputs;

	/** Cycles through the outputs, stores the output we want to put mItem on. Index is for the mOutputs array. */
	UPROPERTY( SaveGame, Meta = (NoAutoJson) )
	int32 mCurrentOutputIndex;

private:
	friend class AFGAttachmentSplitterHologram;

	UPROPERTY()
	TArray< FConnectionItemStruct > mDistributionTable;
};
