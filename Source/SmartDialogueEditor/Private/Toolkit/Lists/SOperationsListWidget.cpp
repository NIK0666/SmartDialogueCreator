// 


#include "SOperationsListWidget.h"
#include "Rows/SOperationsListRow.h"
#include "Toolkit/FSmartDialogueEditor.h"

void SOperationsListWidget::Construct(const FArguments& InArgs)
{
	bIsExecution = InArgs._bIsExecution;
	SBaseListWidget::Construct(SBaseListWidget::FArguments()
		.Title(InArgs._Title)
		.Editor(InArgs._Editor));
}

TSharedRef<SWidget> SOperationsListWidget::GetItemContent(const FListItemData& Item)
{
	return SNew(SOperationsListRow)
		.Item(Item)
		.Editor(Editor)
		.bIsExecution(bIsExecution);
}

TArray<TSharedPtr<FString>> SOperationsListWidget::GetAllStrings()
{	
	return Editor->GetAllVariablesList();
}

FReply SOperationsListWidget::OnContextMenuItemClicked(const FString& Item)
{
	
	Data.Add({Item});
	UpdateData(Data);
	
	return SBaseListWidget::OnContextMenuItemClicked(Item);
}