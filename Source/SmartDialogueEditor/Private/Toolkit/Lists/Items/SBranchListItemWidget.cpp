// SBranchListItemWidget.cpp


#include "SBranchListItemWidget.h"
#include "EditorStyleSet.h"
#include "SmartDialogue.h"
#include "Toolkit/FSmartDialogueEditor.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"

void SBranchListItemWidget::Construct(const FArguments& InArgs)
{
	SBaseListItemWidget::Construct(SBaseListItemWidget::FArguments()
		.Item(InArgs._Item)
		.Editor(InArgs._Editor));
	
	bIsShowed = InArgs._bIsShowed;
	OnChangeClicked = InArgs._OnChangeClicked;
}

TSharedRef<SWidget> SBranchListItemWidget::GetContent()
{
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(Item.Name))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.ButtonStyle(FEditorStyle::Get(), "FlatButton.Primary")
			.ContentPadding(FMargin(0.5f, 1, 0.5f, 1))
			.OnClicked(this, &SBranchListItemWidget::OnChangeButtonClicked)
			[
				SNew(SImage)
				.Image(FEditorStyle::GetBrush("Icons.Edit"))
			]
		];
}

FReply SBranchListItemWidget::OnChangeButtonClicked()
{
	if (OnChangeClicked.IsBound())
	{
		OnChangeClicked.Execute();
	}
	return FReply::Handled();
}
