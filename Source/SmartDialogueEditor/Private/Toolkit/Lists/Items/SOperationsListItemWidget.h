// SOperationsListItemWidget.h
#pragma once

#include "SBaseListItemWidget.h"
#include "SmartDialogueData.h"

class SOperationsListItemWidget : public SBaseListItemWidget
{
public:
	SLATE_BEGIN_ARGS(SOperationsListItemWidget) {}
	SLATE_ARGUMENT(FListItemData, Item)
	SLATE_ARGUMENT(TWeakPtr<FSmartDialogueEditor>, Editor)
	SLATE_ARGUMENT(bool, bIsExecution)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	virtual TSharedRef<SWidget> GetContent() override;

	TArray<FString> GetValueNames();

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