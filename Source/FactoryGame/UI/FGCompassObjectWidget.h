// Copyright 2016-2018 Coffee Stain Studios. All Rights Reserved.

#pragma once
#include "UObject/Class.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../FGActorRepresentation.h"
#include "FGCompassObjectWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACTORYGAME_API UFGCompassObjectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct();
	virtual void Tick( FGeometry MyGeometry, float InDeltaTime );
	
	/** Updates the position and tint in the compass for this CompassObject. Should only be called from within the CompassWidget */
	void UpdatePositionInCompass(float halfWidth, FVector2D origin = FVector2D::ZeroVector);

	/** Sets this CompassObject as a static direction in relation to the player's location (e.g. cardinal coordinates). Must be normalized! */
	UFUNCTION( BlueprintCallable, Category = "Compass" )
	void SetAsStaticDirection( FVector direction ) { mStaticDirection = direction; }

	/** Setter for the Actor Representations that this CompassObject is set to represent */
	UFUNCTION( BlueprintCallable, Category = "Compass" )
	void SetActorRepresentation( class UFGActorRepresentation* actorRepresentation );

	/** Getter for the Actor Representation that this CompassObject is set to represent */
	UFUNCTION( BlueprintPure, Category = "Compass" )
	FORCEINLINE class UFGActorRepresentation* GetActorRepresentation() { return mActorRepresentation; }

	/** This function is called when the compass object is added to the compass. This function should deal with layouts, anchoring, positioning, etc. */
	UFUNCTION( BlueprintNativeEvent, Category = "Compass" )
	void OnCompassObjectAddedToPanel( UCanvasPanelSlot* parentSlot );

	/** This function is called when the representation of the compass object is updated. */
	UFUNCTION( BlueprintNativeEvent, Category = "Compass" )
	void OnCompassObjectUpdated();

	/** This function is called when the representation of the compass object is updated. */
	UFUNCTION( BlueprintImplementableEvent, Category = "Compass" )
	void OnObjectCentered( bool centered );

	FORCEINLINE FVector2D GetLastUpdatedPosition() const { return mlastUpdatedPosition; }

	UFUNCTION( BlueprintPure, Category = "Compass" )
	FORCEINLINE float GetAlphaAmount() const { return mAlphaAmount; }

	UFUNCTION( BlueprintPure, Category = "Compass" )
	FORCEINLINE float GetBlockingAmount() const { return mBlockingAlphaAmount; }

	FORCEINLINE bool GetCheckForBlockingInCompass() const { return mCheckForBlockingInCompass; }

	float GetRepresentationDistanceFromPlayer();
	void SetIsBlocking( bool value ) { mIsBlocking = value; }
	
	UFUNCTION( BlueprintPure, Category = "Compass" )
	FORCEINLINE ERepresentationType GetRepresentationType() const { return mRepresentationType;  }

public:
	UPROPERTY( BlueprintReadWrite, Category = "Compass" )
	class UTexture2D* mTexture;


	UPROPERTY( BlueprintReadWrite, Category = "Compass" )
	FText mText;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Compass" )
	FVector2D mPositionOffset;

protected:
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Compass" )
	bool mClampPosition;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Compass" )
	bool mShouldFadeInEdges;

	UPROPERTY( BlueprintReadOnly, Meta = ( BindWidget ) )
	class UImage* mImage;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Compass" )
	bool mCheckForBlockingInCompass;

	UFUNCTION( BlueprintCallable, Category = "Compass" )
	FVector GetDirectionFromLocation( FVector location );

	UFUNCTION( BlueprintCallable, Category = "Compass" )
	float GetAngleFromDirection( FVector direction );

protected:
	UPROPERTY( BlueprintReadWrite, Meta = ( ExposeOnSpawn ), Category = "Compass" )
	class UFGActorRepresentation* mActorRepresentation;

	UPROPERTY( BlueprintReadOnly )
	FVector mStaticDirection;

	UPROPERTY( BlueprintReadOnly, Category = "Compass" )
	bool mIsBlocking;

private:
	float mAlphaAmount;
	float mBlockingAlphaAmount;

	FVector2D mlastUpdatedPosition;

	ERepresentationType mRepresentationType;
};
