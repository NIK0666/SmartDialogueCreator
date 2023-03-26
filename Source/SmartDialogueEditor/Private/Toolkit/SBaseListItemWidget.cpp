// SBaseListItemWidget.cpp

#include "SBaseListItemWidget.h"
#include "EditorStyleSet.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"

void SBaseListItemWidget::Construct(const FArguments& InArgs)
{
	Item = InArgs._Item;
	OnChangeClicked = InArgs._OnChangeClicked;
	OnRemoveClicked = InArgs._OnRemoveClicked;

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Item))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.ButtonStyle(FEditorStyle::Get(), "FlatButton.Primary")
			.ContentPadding(FMargin(0.5f, 1, 0.5f, 1))
			.OnClicked(this, &SBaseListItemWidget::OnChangeButtonClicked)
			[
				SNew(SImage)
				.Image(FEditorStyle::GetBrush("Icons.Edit"))
			]
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.ButtonStyle(FEditorStyle::Get(), "FlatButton.Danger")
			.ContentPadding(FMargin(0.5f, 1, 0.5f, 1))
			.OnClicked(this, &SBaseListItemWidget::OnRemoveButtonClicked)
			[
				SNew(SImage)
				.Image(FEditorStyle::GetBrush("Cross"))
			]
		]
	];
}

FReply SBaseListItemWidget::OnChangeButtonClicked()
{
	OnChangeClicked.ExecuteIfBound();
	return FReply::Handled();
}

FReply SBaseListItemWidget::OnRemoveButtonClicked()
{
	OnRemoveClicked.ExecuteIfBound();
	return FReply::Handled();
}