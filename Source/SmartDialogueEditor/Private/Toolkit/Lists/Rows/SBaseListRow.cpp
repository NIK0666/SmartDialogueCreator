// SBaseListRow.cpp

#include "SBaseListRow.h"
#include "EditorStyleSet.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"

void SBaseListRow::Construct(const FArguments& InArgs)
{
	Item = InArgs._Item;
	Editor = InArgs._Editor;
	OnRemoveItemRequested = InArgs._OnRemoveItemRequested;
	
	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			GetContent()
		]	
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.ButtonStyle(FAppStyle::Get(), "FlatButton")
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.OnClicked(this, &SBaseListRow::RemoveItem)
			[
				SNew(SImage)
				.Image(FAppStyle::GetBrush("Cross"))
			]
		]	
	];
}

FReply SBaseListRow::RemoveItem()
{
	OnRemoveItemRequested.ExecuteIfBound(Item);
	return FReply::Handled();
}

TSharedRef<SWidget> SBaseListRow::GetContent()
{
	return SNew(STextBlock)
		.Text(FText::FromString(Item.Name));
}