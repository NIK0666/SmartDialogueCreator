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
	SLATE_ARGUMENT(TSharedPtr<FSmartDialogueBranch>, BranchPtr) 
	SLATE_ARGUMENT(TWeakPtr<FSmartDialogueEditor>, Editor)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	void OnBranchNameTextCommitted(const FText& NewText, ETextCommit::Type CommitType);
	void OnBranchTextTextCommitted(const FText& NewText, ETextCommit::Type CommitType);
	
	TSharedPtr<FSmartDialogueBranch> BranchPtr;
	TWeakPtr<FSmartDialogueEditor> Editor;
	TSharedPtr<SEditableTextBox> BranchNameTextBox;
	TSharedPtr<SEditableTextBox> BranchTextTextBox;
};