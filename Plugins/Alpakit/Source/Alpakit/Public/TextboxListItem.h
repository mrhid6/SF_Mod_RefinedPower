#pragma once

#include "EditableItem.h"
//Slate
#include "Slate.h"

class STextboxListItem : public SCompoundWidget
{
	//Slate Init
public:
	SLATE_BEGIN_ARGS(STextboxListItem) : _Item() //, _bKeepVisible(false)
	{}

	SLATE_ARGUMENT(TSharedPtr<FEditableItem>, Item)

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
	TSharedPtr<SEditableTextBox> TextBox;
	
	// Bindings
	TSharedPtr<FEditableItem> Item;
	FText Text;
	FText GetText() const { return Text; }
	TAttribute<FText> Text_Bind;

	// Functions
	void TextChanged(const FText& InText) { Item->Text = InText.ToString(); Text = InText; }
};