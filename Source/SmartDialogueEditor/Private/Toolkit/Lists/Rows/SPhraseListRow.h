// 

#pragma once

#include "CoreMinimal.h"
#include "SmartDialogueData.h"
// #include "SPhraseListRow.generated.h"

class FSmartDialogueEditor;

class SPhraseListRow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPhraseListRow) {}
	SLATE_ARGUMENT(TSharedPtr<FSmartDialogueEditor>, SmartDialogueEditor)
	SLATE_ARGUMENT(FSmartDialoguePhrases, SmartDialoguePhrase)
	SLATE_END_ARGS() 


	void Construct(const FArguments& InArgs);

private:
	TArray<TSharedPtr<FString>> CharacterOptions;
	TArray<TSharedPtr<FString>> VarOptions;
	TArray<TSharedPtr<FString>> ComparisonOptions;

	TSharedPtr<SComboBox<TSharedPtr<FString>>> CharacterComboBox;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> VarComboBox;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> ComparisonComboBox;

	TSharedRef<SWidget> GenerateCharacterOption(TSharedPtr<FString> Option);
	TSharedRef<SWidget> GenerateVarOption(TSharedPtr<FString> Option);
	TSharedRef<SWidget> GenerateComparisonOption(TSharedPtr<FString> Option);

	void OnCharacterSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	void OnVarSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	void OnComparisonSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	void OnOrCheckStateChanged(ECheckBoxState NewState);
	FReply OnSettingsButtonClicked();
	FReply OnHandButtonClicked();
	FReply OnDeleteButtonClicked();
	void OnMultiLineTextChanged(const FText& InText);

	
	void UpdateCharacterOptions();
	void UpdateVarOptions();
	
	TSharedPtr<FSmartDialogueEditor> SmartDialogueEditor;
	FSmartDialoguePhrases SmartDialoguePhrase;
};
