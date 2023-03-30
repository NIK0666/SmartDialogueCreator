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
			.ButtonStyle(FEditorStyle::Get(), "FlatButton.Danger")
			.ContentPadding(FMargin(0.5f, 1, 0.5f, 1))
			.OnClicked(this, &SBaseListRow::RemoveItem)
			[
				SNew(SImage)
				.Image(FEditorStyle::GetBrush("Cross"))
			]
		]
	];
}

FReply SBaseListRow::RemoveItem()
{
	return FReply::Handled();
}

TSharedRef<SWidget> SBaseListRow::GetContent()
{
	return SNew(STextBlock)
		.Text(FText::FromString(Item.Name));
}