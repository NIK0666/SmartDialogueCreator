//SBranchInfoWidget.h

#pragma once

#include "SmartDialogueData.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

class FSmartDialogueEditor;

class SBranchInfoWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBranchInfoWidget) {}
	SLATE_ARGUMENT(FSmartDialogueBranch, Branch)
	SLATE_ARGUMENT(TWeakPtr<FSmartDialogueEditor>, Editor)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	void OnBranchNameTextCommitted(const FText& NewText, ETextCommit::Type CommitType);
	
	FSmartDialogueBranch Branch;
	TWeakPtr<FSmartDialogueEditor> Editor;
	TSharedPtr<SEditableTextBox> BranchNameTextBox;
};
