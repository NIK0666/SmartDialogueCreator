// SBranchPhrasesWidget.h

#pragma once

#include "SmartDialogueData.h"
#include "Lists/Rows/SPhraseListRow.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

class FSmartDialogueEditor;

class SBranchPhrasesWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBranchPhrasesWidget) {}
	SLATE_ARGUMENT(FSmartDialogueEditor*, SmartDialogueEditor)
	SLATE_END_ARGS()

	
	void Construct(const FArguments& InArgs);
	void RemovePhraseRow(int32 PhraseIndex);
	
	void DragProcess(TSharedPtr<SPhraseListRow> DraggedWidget, const FVector2D& MousePosition);
	void DragEnd(TSharedPtr<SPhraseListRow> DraggedWidget, const FVector2D& MousePosition);

private:
	FSmartDialogueEditor* SmartDialogueEditor = nullptr;
	void OnBranchSelected(FSmartDialogueBranch& SmartDialogueBranch);
	void OnResetSelectedBranch();
	void UpdatePhrases();
	
	TArray<TSharedPtr<SPhraseListRow>> PhrasesWidgets;
	TSharedPtr<SPhraseListRow> CurrentlyHighlightedWidget;
	
	TSharedPtr<SVerticalBox> PhrasesVBox;
	TSharedPtr<SButton> AddNewPhraseButton;
	TSharedPtr<SButton> AddFirstHeroPhraseButton;

};
