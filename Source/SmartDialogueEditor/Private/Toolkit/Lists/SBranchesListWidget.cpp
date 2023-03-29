// SBranchesListWidget.h

#include "SBranchesListWidget.h"
#include "Rows/SBranchListRow.h"
#include "Toolkit/FSmartDialogueEditor.h"

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
	
}

FReply SBranchesListWidget::OnChangeButtonClicked()
{
	ShowSelectionMenu();
	return FReply::Handled();
}

TArray<TSharedPtr<FString>> SBranchesListWidget::GetAllStrings()
{	
	return Editor.Get()->GetAllBranchesList();
}

FReply SBranchesListWidget::OnContextMenuItemClicked(const FString& Item)
{
	for (auto Element : Data)
	{
		if (Element.Name == Item)
		{
			return FReply::Handled();
		}
	}
	
	Data.Add({Item});
	UpdateData(Data);
	return SBaseListWidget::OnContextMenuItemClicked(Item);
}

