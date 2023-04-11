// 

#pragma once

#include "CoreMinimal.h"
#include "SmartDialogueData.h"
// #include "SPhraseListRow.generated.h"

class SVarComboBox;
class SCharacterComboBox;
class SMultiLineEditableTextBox;
class FSmartDialogueEditor;
class SGridPanel;

class SPhraseListRow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPhraseListRow) {}
	SLATE_ARGUMENT(FSmartDialogueEditor*, SmartDialogueEditor)
	SLATE_ARGUMENT(int32, PhraseIndex)
	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);

	FSmartDialoguePhrase* GetPhrasePtr() const;
	int32 GetPhraseIndex() { return PhraseIndex; }
	void UnderDragState(bool bIsNewState);
	void DraggedState(bool bIsNewState);

private:
	TArray<TSharedPtr<FString>> CharacterOptions;
	TArray<TSharedPtr<FString>> VarOptions;
	TArray<TSharedPtr<FString>> ComparisonOptions;

	TSharedPtr<SCharacterComboBox> CharacterComboBox;
	TSharedPtr<SGridPanel> CustomParamsGrid;
	TSharedPtr<SVarComboBox> VarComboBox;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> ComparisonComboBox;
	TSharedPtr<STextBlock> ComparisonTextBlock;
	TSharedPtr<SMultiLineEditableTextBox> MultiLineEditableTextBox;
	TSharedPtr<SButton> GrabButton;


	TSharedRef<SWidget> GenerateCharacterOption(TSharedPtr<FString> Option);
	TSharedRef<SWidget> GenerateVarOption(TSharedPtr<FString> Option);
	TSharedRef<SWidget> GenerateComparisonOption(TSharedPtr<FString> Option);

	void OnCharacterSelected(TSharedPtr<FString> NewSelection);
	void OnVarSelected(TSharedPtr<FString> NewSelection);
	void OnComparisonSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	void OnOrCheckStateChanged(ECheckBoxState NewState);
	void OnParameterChanged(const FText& Text, FString ParamKey);
	void UpdateCustomParamsGrid();
	FReply OnDeleteButtonClicked();
	void OnMultiLineTextChanged(const FText& InText);
	
	FText GetCurrentCharacterText() const;
	FText GetCurrentVarText() const;
	FText GetCurrentComparisonText() const;

	FReply OnMultiLineKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent);
	EVisibility GetComparisonVisibility() const;
	FSlateColor GetBackgroundColor() const;
	FText GetCurrentText() const;
	
	FText GetAnimationText() const;
	void OnAnimationTextChanged(const FText& Text);
	FText GetCompareValueText() const;
	void OnCompareValueTextChanged(const FText& Text);

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;


	FSmartDialogueEditor* SmartDialogueEditor = nullptr;
	int32 PhraseIndex = -1;

	bool bUnderDrag = false;
	bool bDragged = false;
};