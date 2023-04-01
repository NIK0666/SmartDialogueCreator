// 


#include "SOperationsListWidget.h"

#include "SmartDialogue.h"
#include "Rows/SOperationsListRow.h"
#include "Toolkit/FSmartDialogueEditor.h"

void SOperationsListWidget::Construct(const FArguments& InArgs)
{
	bIsExecution = InArgs._bIsExecution;
	SBaseListWidget::Construct(SBaseListWidget::FArguments()
		.Title(InArgs._Title)
		.Editor(InArgs._Editor));
}

void SOperationsListWidget::RemoveItem(const FListItemData& ItemToRemove)
{
	for (int32 i = 0; i < Data.Num(); i++)
	{
		if (Data[i] == ItemToRemove)
		{
			if (bIsExecution)
			{
				Editor->GetDialogue()->RemoveVarOperation(Editor->GetSelectedBranchName(), i);
			}
			else
			{
				Editor->GetDialogue()->RemoveIfOperation(Editor->GetSelectedBranchName(), i);
			}
		}
	}
	
	SBaseListWidget::RemoveItem(ItemToRemove);
}

TSharedRef<SWidget> SOperationsListWidget::GetItemContent(const FListItemData& Item)
{
	return SNew(SOperationsListRow)
		.Item(Item)
		.Editor(Editor)
		.bIsExecution(bIsExecution)
		.OnRemoveItemRequested(this, &SOperationsListWidget::RemoveItem);
}

TArray<TSharedPtr<FString>> SOperationsListWidget::GetAllStrings()
{	
	return Editor->GetAllVariablesList();
}

FReply SOperationsListWidget::OnContextMenuItemClicked(const FString& Item)
{
	




	if (Editor->GetDialogue())
	{
		if (bIsExecution)
		{
			FListItemData NewItem = {Item, "=", 0};
			Data.Add(NewItem);
			Editor->GetDialogue()->AddVarOperation(Editor->GetSelectedBranchName(), NewItem.Name, NewItem.OperationString, NewItem.Value);
		}
		else
		{
			FListItemData NewItem = {Item, "==", 0};
			Data.Add(NewItem);
			Editor->GetDialogue()->AddIfOperation(Editor->GetSelectedBranchName(), NewItem.Name, NewItem.OperationString, NewItem.Value);
		}		
	}

	UpdateData(Data);
	
	return SBaseListWidget::OnContextMenuItemClicked(Item);
}