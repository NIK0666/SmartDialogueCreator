//SBranchInfoWidget.h

#pragma once

#include "SmartDialogueData.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

class SEditableTextBox;
class FSmartDialogueEditor;
class USmartDialogue;

class SBranchInfoWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBranchInfoWidget) {}
	SLATE_ARGUMENT(FName, BranchName) 
	SLATE_ARGUMENT(TSharedPtr<FSmartDialogueEditor>, Editor)
	SLATE_END_ARGS()

	
	void Construct(const FArguments& InArgs);

private:
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	
	FText GetBranchText() const;
	FSlateColor GetBackgroundColor() const;

	void OnBranchNameTextCommitted(const FText& NewText, ETextCommit::Type CommitType);
	void OnBranchTextTextCommitted(const FText& NewText, ETextCommit::Type CommitType);
	void OnGrabButtonPressed();
	void OnGrabButtonReleased();
	void SetIsFocused(bool bCond);
	FText GetPlaceholderText() const;

	FName BranchName;
	TSharedPtr<FSmartDialogueEditor> Editor;
	TSharedPtr<SEditableTextBox> BranchNameTextBox;
	TSharedPtr<SEditableTextBox> BranchTextTextBox;
	TSharedPtr<SButton> GrabButton;
	
	USmartDialogue* DialoguePtr = nullptr;
	bool bIsFocused = false;
};