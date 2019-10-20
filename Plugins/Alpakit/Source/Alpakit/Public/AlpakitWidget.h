#pragma once

//Slate
#include "Slate.h"
#include "AlpakitSettings.h"
#include "EditableItem.h"

class SAlpakaWidget : public SCompoundWidget
{
	//Slate Init
public:
	SLATE_BEGIN_ARGS(SAlpakaWidget) //, _bKeepVisible(false)
	{}


	SLATE_END_ARGS()
public:
	//Constructor
	void Construct(const FArguments& InArgs);

	//As Widget
	TSharedRef<class SWidget> AsWidget();

	//~~~ Tick ~~~ 
	//void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);

public:
	// UI
	TSharedPtr<SButton> AlpakitButton;
	TSharedPtr<SButton> SaveSettingsButton;
	TSharedPtr<SListView<TSharedPtr<FEditableItem>>> ListViewWidget;

	// Bindings
	UAlpakitSettings* Settings;

	TArray<TSharedPtr<FEditableItem>> Items;

	// Functions
	FReply ChooseSatisfactoryFolder();
	bool OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, TArray<FString>& OutFileNames);
	bool OpenDirectoryDialog(const FString& DialogTitle, const FString& DefaultPath, FString& OutFileNames);
	FReply Pakit();
	FReply SaveSettings();

	void CookContent();
	void CookDone(FString result, double runtime);

	FReply AddMod();

	/* Adds a new textbox with the string to the list */
	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FEditableItem> Item, const TSharedRef<STableViewBase>& OwnerTable);
};