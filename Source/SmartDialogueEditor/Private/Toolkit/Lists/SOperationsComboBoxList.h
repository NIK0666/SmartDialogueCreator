#pragma once

#include "Toolkit/FSmartDialogueEditor.h"
#include "SmartDialogue.h"

class SOperationsComboBoxList : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SOperationsComboBoxList) {}
	SLATE_ARGUMENT(TArray<FSmartDialogueVars>, InitialVars)
	SLATE_ARGUMENT(FSmartDialogueEditor*, Editor)
	SLATE_ARGUMENT(FText, Title)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void UpdateInitialVars(const TArray<FSmartDialogueVars>& NewInitialVars);

private:
	TArray<FSmartDialogueVars> InitialVars;
	TSharedPtr<SVerticalBox> ListBox;

	TArray<TSharedPtr<FString>> VariableOptions;
	TArray<TSharedPtr<FString>> OperationOptions;

	FString ESmartDialogueOperationToString(ESmartDialogueOperation Operation);
	ESmartDialogueOperation ESmartDialogueOperationFromString(FString& String);
	
	void RefreshList();

	void GetOptions();

	TSharedRef<SWidget> MakeWidgetForVariableOption(TSharedPtr<FString> InOption);
	TSharedRef<SWidget> MakeWidgetForOperationOption(TSharedPtr<FString> InOption);

	FReply OnAddButtonClicked();

	void OnVariableSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo, int32 Index);
	void OnOperationSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo, int32 Index);
	void OnValueTextChanged(const FText& NewValue, int32 Index);

	FReply OnDeleteButtonClicked(int32 Index);

	FSmartDialogueEditor* Editor;
};
