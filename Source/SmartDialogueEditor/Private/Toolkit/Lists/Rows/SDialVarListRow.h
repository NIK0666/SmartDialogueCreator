// 

#pragma once

#include "CoreMinimal.h"
#include "SmartDialogueData.h"

DECLARE_DELEGATE_OneParam(FVariableDataChanged, const FVariableData& UpdatedVariable)

class SDialConfigWidget;
class SDialVarListRow : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_OneParam(FOnVarDeleted, TSharedPtr<FString>)

	
	SLATE_BEGIN_ARGS(SDialVarListRow) {}
	SLATE_ARGUMENT(FString, VarKey)
	SLATE_ARGUMENT(int32, VarValue)
	SLATE_ARGUMENT(FString, VarDesc)
	SLATE_EVENT(FVariableDataChanged, OnChanged)
	SLATE_EVENT(FSimpleDelegate, OnDeleteButtonClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
	FSimpleDelegate OnDeleteButtonClicked;
	FVariableDataChanged OnChanged;

private:
	TAttribute<FString> VarKey;
	TAttribute<int32> VarValue;
	TAttribute<FString> VarDesc;
	
	TSharedPtr<SNumericEntryBox<int32>> DefaultValueNumericEntryBox;
	TSharedPtr<SEditableTextBox> DescriptionEditableTextBox;
	TSharedPtr<SEditableTextBox> KeyEditableTextBox;
};

