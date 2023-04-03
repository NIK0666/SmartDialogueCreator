// 

#pragma once

#include "CoreMinimal.h"
#include "SmartDialogueData.h"
// #include "SPhraseListRow.generated.h"

class SVarComboBox;
class SCharacterComboBox;
class SMultiLineEditableTextBox;
class FSmartDialogueEditor;

class SPhraseListRow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPhraseListRow) {}
	SLATE_ARGUMENT(FSmartDialogueEditor*, SmartDialogueEditor)
	SLATE_ARGUMENT(int32, PhraseIndex)
	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);

	FSmartDialoguePhrase* GetPhrasePtr() const;

private:
	TArray<TSharedPtr<FString>> CharacterOptions;
	TArray<TSharedPtr<FString>> VarOptions;
	TArray<TSharedPtr<FString>> ComparisonOptions;

	TSharedPtr<SCharacterComboBox> CharacterComboBox;
	TSharedPtr<SVarComboBox> VarComboBox;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> ComparisonComboBox;
	TSharedPtr<STextBlock> ComparisonTextBlock;
	TSharedPtr<SMultiLineEditableTextBox> MultiLineEditableTextBox;

	TSharedRef<SWidget> GenerateCharacterOption(TSharedPtr<FString> Option);
	TSharedRef<SWidget> GenerateVarOption(TSharedPtr<FString> Option);
	TSharedRef<SWidget> GenerateComparisonOption(TSharedPtr<FString> Option);

	void OnCharacterSelected(TSharedPtr<FString> NewSelection);
	void OnVarSelected(TSharedPtr<FString> NewSelection);
	void OnComparisonSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	void OnOrCheckStateChanged(ECheckBoxState NewState);
	FReply OnSettingsButtonClicked();
	FReply OnHandButtonClicked();
	FReply OnDeleteButtonClicked();
	void OnMultiLineTextChanged(const FText& InText);
	
	FText GetCurrentCharacterText() const;
	FText GetCurrentVarText() const;
	FText GetCurrentComparisonText() const;

	FReply OnMultiLineKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent);
	EVisibility GetComparisonVisibility() const;
	FText GetCurrentText() const;
	
	FText GetAnimationText() const;
	void OnAnimationTextChanged(const FText& Text);
	FText GetCompareValueText() const;
	void OnCompareValueTextChanged(const FText& Text);


	FSmartDialogueEditor* SmartDialogueEditor = nullptr;
	FSmartDialoguePhrase* SmartDialoguePhrasePtr = nullptr;
	int32 PhraseIndex = -1;
};