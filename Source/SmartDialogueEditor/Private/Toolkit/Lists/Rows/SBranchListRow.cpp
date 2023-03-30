// SBranchListRow.cpp


#include "SBranchListRow.h"
#include "EditorStyleSet.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"

void SBranchListRow::Construct(const FArguments& InArgs)
{
	SBaseListRow::Construct(SBaseListRow::FArguments()
		.Item(InArgs._Item)
		.Editor(InArgs._Editor));
	
	bIsShowed = InArgs._bIsShowed;
	OnChangeClicked = InArgs._OnChangeClicked;
}

TSharedRef<SWidget> SBranchListRow::GetContent()
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
			.OnClicked(this, &SBranchListRow::OnChangeButtonClicked)
			[
				SNew(SImage)
				.Image(FEditorStyle::GetBrush("Icons.Edit"))
			]
		];
}

FReply SBranchListRow::RemoveItem()
{
	UE_LOG(LogTemp, Log, TEXT("SBranchListRow::RemoveItem"));
	return SBaseListRow::RemoveItem();
}

FReply SBranchListRow::OnChangeButtonClicked()
{
	if (OnChangeClicked.IsBound())
	{
		OnChangeClicked.Execute();
	}
	return FReply::Handled();
}