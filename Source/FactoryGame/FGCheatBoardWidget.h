//Copyright 2016 Coffee Stain Studios.All Rights Reserved.

#pragma once
#include "Array.h"
#include "UnrealString.h"
#include "UMG.h"

#include "FGCheatManager.h"
#include "SlateBasics.h"

#if WITH_CHEATS

/**
*
*/

class FACTORYGAME_API SFGCheatBoardWidget : public SCompoundWidget
{
	/** Cache the cheat manager */
	SLATE_BEGIN_ARGS( SFGCheatBoardWidget ) : _mCachedCheatManager(){}
	SLATE_ARGUMENT( TWeakObjectPtr< UFGCheatManager >, mCachedCheatManager )
	SLATE_END_ARGS()

public:

	/** Constructs the base Slate UI */
	void Construct( const FArguments& InArgs );

	/** Catch input in this widget */
	virtual FReply OnKeyDown( const FGeometry& myGeometry, const FKeyEvent& inKeyEvent );

	/** Overloaded to be able to take keyboard focus */
	virtual bool SupportsKeyboardFocus() const;

	/** Shut it down */
	void CloseCheatBoard();

	virtual FReply OnFocusReceived( const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent ) override;

private:

	/** This is just the base, that is supposed to show propts for the user to input data.
	 * this is overloaded per property type */
	void ShowInputWindowFor( UProperty* prop );

	/** Propmts the user with true and false inputs */
	void ShowInputWindowFor( UBoolProperty* prop );

	/** Propmts the user with true and false inputs */
	void ShowInputWindowFor( UNumericProperty* prop );
	
	/** Propmts the user to input a resource class */
	void ShowInputWindowFor( UClass* inClass );

	/** Propmts the user to input a text, FString or UText. (No FName yet) */
	void ShowInputWindowFor( UStrProperty* prop );

	/** Propmts the user to input a color */
	void ShowInputWindowFor( FLinearColor color );	

	/** This finds all the functions in UFGCheatManager */
	TArray< UFunction* > GetAllCheats();

	/** When we click on a cheat in the list this get triggered
	 *
	 * @ param func - The cheat function the player clicked
	 */
	FReply OnCheatClicked( UFunction* func );

	/** Called when the players commits text in the input window */
	void OnInputTextCommited( const FText& InText, ETextCommit::Type InCommitType );

	/** If the user presses on true or false, this triggers and gives us the value */
	FReply OnBoolParm( bool value );

	/** If the user presses on a resource, this triggers and gives us the value */
	FReply OnClassParm( UClass* inClass );

	/** Result from the colorpicker */
	void OnSetColorFromColorPicker( FLinearColor newColor );

	/** Get the next property for mFunctionWaitingForParms, where the current is mPropertyWaitingForInput*/
	UProperty* GetNextProperty();

public:

	/** Our cached cheat manager */
	TWeakObjectPtr< UFGCheatManager > mCachedCheatManager;

	/** Indicates if this widgets want to be destroyed */
	bool mMarkedForDestroy;
private:
	
	/** Cached slot for the list of cheat buttons */
	SOverlay::FOverlaySlot* mCheatButtonSlot;

	/** Cached slot for the possible input buttons */
	SOverlay::FOverlaySlot* mInputSlot;

	/** This function is waiting for input from the user */
	UFunction* mFunctionWaitingForParms;

	/** The parameters that should be proccessed by the function
	  * I tried to get this board to support multiple parameters,
	  * but it isn't working. The data in mParameters get jumbled up when
	  * trying to write more properties to it */
	uint8* mParameters;

	/** This is the property we asked for input about */
	UProperty* mPropertyWaitingForInput;

	/** The textstyle used in this widget */
	FTextBlockStyle* mButtonTextStyle;

	/** The text style in the box where the user inputs shit */
	FEditableTextBoxStyle* mEditableTextStyle;

	TArray< TSharedRef< SWidget > > mButtons;

	/*UPROPERTY( Config )
	TArray< UClass* > mPopularUClassChoices;*/

};
#endif