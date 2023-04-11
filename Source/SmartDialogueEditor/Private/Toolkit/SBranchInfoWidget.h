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
	virtual ~SBranchInfoWidget() override;

	FName GetBranchName() const;
	FText GetBranchNameAsText() const;
	
	void SetEditableTextFocus(bool bIsTextFocused);
	void UnderDragState(bool bIsUnderDrag);
	void DraggedState(bool bIsDragged);

private:
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	
	UFUNCTION()
	void OnBranchRenamed(FName OldName, FName NewName);

	FText GetBranchText() const;
	FSlateColor GetBackgroundColor() const;

	void OnBranchNameTextCommitted(const FText& NewText, ETextCommit::Type CommitType);
	void OnBranchTextTextCommitted(const FText& NewText, ETextCommit::Type CommitType);
	FReply OnRemoveBranchButtonClicked();

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	TSharedPtr<SBranchInfoWidget> GetParentBranchInfoWidget(const TSharedPtr<SWidget>& Widget);

	
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
	FDelegateHandle BranchRenamedHandle;

	
	USmartDialogue* DialoguePtr = nullptr;
	bool bIsFocused = false;
	bool bUnderDrag = false;
	bool bDragged = false;
};