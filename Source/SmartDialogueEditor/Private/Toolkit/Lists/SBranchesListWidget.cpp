// SBranchesListWidget.h

#include "SBranchesListWidget.h"

#include "Rows/SBranchListRow.h"

void SBranchesListWidget::Construct(const FArguments& InArgs)
{
	bIsShowed = InArgs._bIsShowed;
	
	SBaseListWidget::Construct(SBaseListWidget::FArguments()
		.Title(InArgs._Title)
		.Editor(InArgs._Editor));
}

TSharedRef<SWidget> SBranchesListWidget::GetItemContent(const FListItemData& Item)
{
	return SNew(SBranchListRow)
		.Item(Item)
		.bIsShowed(bIsShowed)
		.Editor(Editor)
		.OnChangeClicked(this, &SBranchesListWidget::OnChangeButtonClicked);
}

void SBranchesListWidget::ShowSelectionMenu()
{
	// TArray<TSharedPtr<FString>> AllStrings = GetAllStrings();
	// TSharedPtr<SComboBox<TSharedPtr<FString>>> ComboBox;
	// SAssignNew(ComboBox, SComboBox<TSharedPtr<FString>>)
	// 	.OptionsSource(&AllStrings)
	// 	.OnGenerateWidget(this, &SBranchesListWidget::GenerateStringItemWidget)
	// 	.OnSelectionChanged(this, &SBranchesListWidget::OnComboBoxSelectionChanged)
	// 	.Content()
	// 	[
	// 		SNew(STextBlock)
	// 		.Text(NSLOCTEXT("Temp", "SelectItem", "Select an item"))
	// 	];
	//
	// FSlateApplication::Get().PushMenu(
	// 	SharedThis(this),
	// 	FWidgetPath(),
	// 	ComboBox.ToSharedRef(),
	// 	FSlateApplication::Get().GetCursorPos(),
	// 	FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu)
	// );
}

FReply SBranchesListWidget::OnChangeButtonClicked()
{
	ShowSelectionMenu();
	return FReply::Handled();
}