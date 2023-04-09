// 

#pragma once

#include "CoreMinimal.h"
#include "DialogueManager.h"
#include "DialoguePlayerData.h"
#include "Widgets/SCompoundWidget.h"

class SScrollBox;
class SDialogueElementRow;
class FSmartDialogueEditor;
class UDialogueManager;

class SDialoguePlayerTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDialoguePlayerTab) {}
	SLATE_ARGUMENT(FSmartDialogueEditor*, SmartDialogueEditor)
	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);

protected:
	TSharedRef<ITableRow> GenerateDialogueRow(TSharedPtr<FDialogueElementData> InDialogueElementData, const TSharedRef<STableViewBase>& OwnerTable);

	void UpdateAnswers(const TArray<FText>& Answers);
	void OnAnswerSelected(int32 AnswerIndex);

	
	FReply OnPlayClicked();
	FReply OnClearClicked();
	FReply OnStopClicked();
	FReply OnSwitchLocaleClicked();

	TSharedPtr<SScrollBox> DialogueScrollBox;
	TSharedPtr<SScrollBox> AnswerScrollBox;
	FString HeroName = TEXT("Hero");
	
	FSmartDialogueEditor* Editor;
	
	UDialogueManager* DialManager;
	TArray<int32> BranchIndices;

	TArray<TSharedPtr<FDialogueElementData>> DialogueElements;
	TSharedPtr<SListView<TSharedPtr<FDialogueElementData>>> DialogueListView;
	TSharedPtr<SButton> NextPhraseButton;
	TSharedPtr<SWidgetSwitcher> AnswerSwitcher;

	void OnShowBranchOptionsHandler(const TArray<FText>& BranchTexts, const TArray<int32>& BranchIndices);
	void OnHideBranchOptionsHandler();
	void OnShowPhraseHandler(const FText& PhraseText, const FString& SpeakerId);
	void OnCloseDialogueHandler();
	void OnEventTriggeredHandler(const FString& EventName, const FString& EventParam);
	void OnVariableChanged(bool bisPublic, const FString& VarName, int OldValue, int NewValue);

	FReply OnNextPhraseClicked();

	
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

};