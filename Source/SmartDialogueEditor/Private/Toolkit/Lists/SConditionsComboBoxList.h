//

#pragma once

#include "Toolkit/FSmartDialogueEditor.h"

class SConditionsComboBoxList : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SConditionsComboBoxList) {}
	SLATE_ARGUMENT(TArray<FIf>, InitialConditions)
	SLATE_ARGUMENT(FSmartDialogueEditor*, Editor)
	SLATE_ARGUMENT(FText, Title)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void UpdateInitialConditions(const TArray<FIf>& NewInitialConditions);

private:
	TArray<FIf> InitialConditions;
	TSharedPtr<SVerticalBox> ListBox;

	TArray<TSharedPtr<FString>> VariableOptions;
	TArray<TSharedPtr<FString>> OperationOptions;

	FString EnumOperationToString(ESmartDialogueEqualOperation Operation);
	ESmartDialogueEqualOperation EnumOperationFromString(FString& String);
	
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
