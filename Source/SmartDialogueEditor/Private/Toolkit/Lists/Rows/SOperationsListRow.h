// SOperationsListRow.h
#pragma once

#include "SBaseListRow.h"
#include "SmartDialogueData.h"

class SOperationsListRow : public SBaseListRow
{
public:
	SLATE_BEGIN_ARGS(SOperationsListRow) {}
	SLATE_ARGUMENT(FListItemData, Item)
	SLATE_ARGUMENT(TSharedPtr<FSmartDialogueEditor>, Editor)
	SLATE_ARGUMENT(bool, bIsExecution)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	virtual TSharedRef<SWidget> GetContent() override;

	TArray<TSharedPtr<FString>> GetOperations();

	void OnVariableSelected(TSharedPtr<FString> SelectedItem, ESelectInfo::Type SelectInfo);

	void OnOperationSelected(TSharedPtr<FString> SelectedItem, ESelectInfo::Type SelectInfo);

	virtual FReply RemoveItem() override;

private:
	TSharedPtr<SEditableTextBox> InputTextBox;

	TSharedPtr<SComboBox<TSharedPtr<FString>>> VariableComboBox;

	TSharedPtr<SComboBox<TSharedPtr<FString>>> OperationComboBox;

	TArray<TSharedPtr<FString>> VariableOptions;

	TArray<TSharedPtr<FString>> OperationOptions;

	TSharedPtr<STextBlock> VariableButtonText;
	TSharedPtr<STextBlock> OperationButtonText;

	bool bIsExecution;

	TSharedPtr<FString> SelectedVariable;
	
	int32 SelectedEnumIndex = INDEX_NONE;
	

};