// SBranchesListWidget.h

#include "SBranchesListWidget.h"
#include "Rows/SBranchListRow.h"
#include "SmartDialogue.h"
#include "Toolkit/FSmartDialogueEditor.h"

void SBranchesListWidget::Construct(const FArguments& InArgs)
{
	bIsShowed = InArgs._bIsShowed;
	
	SBaseListWidget::Construct(SBaseListWidget::FArguments()
		.Title(InArgs._Title)
		.Editor(InArgs._Editor));

	Editor->OnBranchItemRemoved.AddSP(this, &SBranchesListWidget::OnBranchItemRemoved);
}

TSharedRef<SWidget> SBranchesListWidget::GetItemContent(const FListItemData& Item)
{
	return SNew(SBranchListRow)
		.Item(Item)
		.bIsShowed(bIsShowed)
		.Editor(Editor)
		.OnRemoveItemRequested(this, &SBaseListWidget::RemoveItem);
;
}

TArray<TSharedPtr<FString>> SBranchesListWidget::GetAllStrings()
{	
	return Editor->GetAllBranchesList();
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

	if (Editor->GetDialogue())
	{
		if (bIsShowed)
		{
			Editor->GetDialogue()->AddShowBranch(Editor->GetSelectedBranchName(), Item);
		}
		else
		{
			Editor->GetDialogue()->AddHideBranch(Editor->GetSelectedBranchName(), Item);
		}		
	}
	
	return SBaseListWidget::OnContextMenuItemClicked(Item);
}

void SBranchesListWidget::OnBranchItemRemoved(FName& Name)
{
	for (int32 i = Data.Num() - 1; i >= 0; i--)
	{
		if (Data[i].Name == Name.ToString())
		{
			RemoveItem(Data[i]);
		}
	}
}

