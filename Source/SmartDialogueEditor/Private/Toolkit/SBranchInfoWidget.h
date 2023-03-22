//SBranchInfoWidget.h

#pragma once

#include "SmartDialogueData.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

class FSmartDialogueEditor;
class SEditableTextBox;
class USmartDialogue;

class SBranchInfoWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBranchInfoWidget) {}
	SLATE_ARGUMENT(FName, BranchName) 
	SLATE_ARGUMENT(TWeakPtr<FSmartDialogueEditor>, Editor)
	SLATE_END_ARGS()
	void Construct(const FArguments& InArgs);

private:

	FText GetBranchText() const;
	
	void OnBranchNameTextCommitted(const FText& NewText, ETextCommit::Type CommitType);
	void OnBranchTextTextCommitted(const FText& NewText, ETextCommit::Type CommitType);
	void OnGrabButtonPressed();
	void OnGrabButtonReleased();

	FName BranchName;
	TWeakPtr<FSmartDialogueEditor> Editor;
	TSharedPtr<SEditableTextBox> BranchNameTextBox;
	TSharedPtr<SEditableTextBox> BranchTextTextBox;
	TSharedPtr<SButton> GrabButton;
	
	USmartDialogue* DialoguePtr = nullptr;
};