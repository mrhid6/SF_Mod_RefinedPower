#include "TextboxListItem.h"

void STextboxListItem::Construct(const FArguments& InArgs)
{
	Item = InArgs._Item;
	Text = FText::FromString(*Item->Text);
	Text_Bind.BindRaw(this, &STextboxListItem::GetText);
	ChildSlot
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Center)
	.Padding(2.0f)
	[
		SAssignNew(TextBox, SEditableTextBox)
		.Text(Text_Bind)
		.OnTextChanged(this, &STextboxListItem::TextChanged)
	];
}

TSharedRef<SWidget> STextboxListItem::AsWidget()
{
	return SharedThis(this);
}