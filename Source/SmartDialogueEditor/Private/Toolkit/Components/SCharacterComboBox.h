// SCharacterComboBox.h
#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Toolkit/FSmartDialogueEditor.h"

DECLARE_DELEGATE_OneParam(FOnCharacterSelectedDelegate, TSharedPtr<FString>);

class SCharacterComboBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCharacterComboBox)
		: _SmartDialogueEditor(nullptr)
	{}
	SLATE_ARGUMENT(TSharedPtr<FSmartDialogueEditor>, SmartDialogueEditor)
	SLATE_EVENT(FOnCharacterSelectedDelegate, OnCharacterSelected)
	SLATE_ARGUMENT(FString, DefaultText)
SLATE_END_ARGS()

void Construct(const FArguments& InArgs);

private:
	TArray<TSharedPtr<FString>> CharacterOptions;
	TSharedPtr<FSmartDialogueEditor> SmartDialogueEditor;
	FOnCharacterSelectedDelegate OnCharacterSelected;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> ComboBox;
	TSharedPtr<STextBlock> TextBlock;
	
	FString SelectionItem;

	TSharedRef<SWidget> GenerateCharacterOption(TSharedPtr<FString> Option);
	void OnCharacterSelectedHandler(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	FText GetCurrentCharacterText() const;
	void OnComboBoxOpeningHandler();

	FString DefaultText;
};