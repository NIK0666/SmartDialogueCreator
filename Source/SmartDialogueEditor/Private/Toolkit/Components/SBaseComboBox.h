// SBaseComboBox.h

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Toolkit/FSmartDialogueEditor.h"

DECLARE_DELEGATE_OneParam(FOnItemSelectedDelegate, TSharedPtr<FString>);

class SBaseComboBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBaseComboBox)
		: _SmartDialogueEditor(nullptr)
	{}
	SLATE_ARGUMENT(TSharedPtr<FSmartDialogueEditor>, SmartDialogueEditor)
	SLATE_EVENT(FOnItemSelectedDelegate, OnItemSelected)
	SLATE_ARGUMENT(FString, DefaultText)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);
	
protected:
	TSharedPtr<FSmartDialogueEditor> SmartDialogueEditor;
	TArray<TSharedPtr<FString>> Options;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> ComboBox;
	TSharedPtr<STextBlock> TextBlock;

	FOnItemSelectedDelegate OnItemSelected;
	FString DefaultText;


	TSharedRef<SWidget> GenerateOption(TSharedPtr<FString> Option);
	void OnItemSelectedHandler(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	FText GetCurrentItemText() const;
	void UpdateOptions();
	virtual TArray<TSharedPtr<FString>> GetOptionsList() { return {};}
	void OnComboBoxOpeningHandler();
};