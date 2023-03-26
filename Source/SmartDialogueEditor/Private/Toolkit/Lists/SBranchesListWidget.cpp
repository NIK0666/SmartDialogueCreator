// SBranchesListWidget.h

#include "SBranchesListWidget.h"

#include "SBranchListItemWidget.h"

void SBranchesListWidget::Construct(const FArguments& InArgs)
{
	SVerticalListWidget::Construct(InArgs);
}

TSharedRef<SWidget> SBranchesListWidget::GetItemContent(const FString& Item)
{
	return SNew(SBranchListItemWidget)
		.Item(Item)
		.OnChangeClicked(this, &SBranchesListWidget::OnChangeButtonClicked);
}

void SBranchesListWidget::ShowSelectionMenu()
{
	TArray<TSharedPtr<FString>> AllStrings = GetAllStrings();
	TSharedPtr<SComboBox<TSharedPtr<FString>>> ComboBox;
	SAssignNew(ComboBox, SComboBox<TSharedPtr<FString>>)
		.OptionsSource(&AllStrings)
		.OnGenerateWidget(this, &SBranchesListWidget::GenerateStringItemWidget)
		.OnSelectionChanged(this, &SBranchesListWidget::OnComboBoxSelectionChanged)
		.Content()
		[
			SNew(STextBlock)
			.Text(NSLOCTEXT("Temp", "SelectItem", "Select an item"))
		];

	FSlateApplication::Get().PushMenu(
		SharedThis(this),
		FWidgetPath(),
		ComboBox.ToSharedRef(),
		FSlateApplication::Get().GetCursorPos(),
		FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu)
	);
}

FReply SBranchesListWidget::OnChangeButtonClicked()
{
	ShowSelectionMenu();
	return FReply::Handled();
}
