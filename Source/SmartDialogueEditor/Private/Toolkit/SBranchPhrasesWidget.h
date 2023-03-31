// SBranchPhrasesWidget.h

#pragma once

#include "SmartDialogueData.h"
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

private:
	FSmartDialogueEditor* SmartDialogueEditor = nullptr;
	void OnBranchSelected(FSmartDialogueBranch& SmartDialogueBranch);
	void UpdatePhrases();
	
	TSharedPtr<SVerticalBox> PhrasesVBox;
	TSharedPtr<SButton> AddNewPhraseButton;
	TSharedPtr<SButton> AddFirstHeroPhraseButton;

};