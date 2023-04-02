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
	SLATE_ARGUMENT(FSmartDialogueEditor*, Editor)
	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);

	FName GetBranchName() const;
	
	void SetEditableTextFocus(bool bIsTextFocused);

private:
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	
	FText GetBranchText() const;
	FSlateColor GetBackgroundColor() const;

	void OnBranchNameTextCommitted(const FText& NewText, ETextCommit::Type CommitType);
	void OnBranchTextTextCommitted(const FText& NewText, ETextCommit::Type CommitType);
	FReply OnRemoveBranchButtonClicked();

	void OnGrabButtonPressed();
	void OnGrabButtonReleased();
	void SetIsFocused(bool bCond);
	FText GetPlaceholderText() const;
	FReply OnBranchNameTextBoxKeyDown(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent);
	FReply OnBranchTextTextBoxKeyDown(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent);
	void MoveFocusToNextTextBox(int32 Direction);

	FName BranchName;
	FSmartDialogueEditor* Editor = nullptr;
	TSharedPtr<SEditableTextBox> BranchNameTextBox;
	TSharedPtr<SEditableTextBox> BranchTextTextBox;
	TSharedPtr<SButton> GrabButton;
	
	USmartDialogue* DialoguePtr = nullptr;
	bool bIsFocused = false;
};