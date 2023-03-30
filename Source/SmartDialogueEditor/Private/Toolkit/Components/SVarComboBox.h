// SVarComboBox.h

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Toolkit/FSmartDialogueEditor.h"

DECLARE_DELEGATE_OneParam(FOnVarSelected, TSharedPtr<FString>)

class SVarComboBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SVarComboBox)
	{
	}
	SLATE_ARGUMENT(TSharedPtr<FSmartDialogueEditor>, SmartDialogueEditor)
	SLATE_EVENT(FOnVarSelected, OnVarSelected)
	SLATE_ARGUMENT(FString, DefaultText)
SLATE_END_ARGS()

void Construct(const FArguments& InArgs);

private:
	TSharedPtr<FSmartDialogueEditor> SmartDialogueEditor;
	TArray<TSharedPtr<FString>> VarOptions;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> ComboBox;
	TSharedPtr<STextBlock> TextBlock;

	FOnVarSelected OnVarSelected;
	FString DefaultText;

	TSharedRef<SWidget> GenerateVarOption(TSharedPtr<FString> Option);
	void OnVarSelectedHandler(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);

	FText GetCurrentVarText() const;
};
