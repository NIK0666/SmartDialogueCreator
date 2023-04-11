// SBranchInfoWidget.cpp

#include "SBranchInfoWidget.h"

#include "FSmartDialogueEditor.h"
#include "SmartDialogue.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "SBranchesWidget.h"
#include "DragDrop/BranchDragDropOperation.h"

#define LOCTEXT_NAMESPACE "SmartDialogueEditor"

FSlateColor SBranchInfoWidget::GetBackgroundColor() const
{
	if (bUnderDrag)
	{
		return FLinearColor(0.88f, 0.88f, 0.0f, 1.f);
	}
	if (bDragged)
	{
		return FLinearColor(0.88f, 0.88f, 0.88f, 1.f);
	}
	
	if (Editor->GetSelectedBranchName() == BranchName)
	{
		return FLinearColor(0.25f, 0.88f, 0.82f, 0.5f);
	}
	
	if (const auto SelectedBranch = Editor->GetSelectedBranch())
	{
		if (SelectedBranch->Hide.Contains(BranchName.ToString()))
		{
			return FLinearColor(0.91f, 0.3f, 0.24f, 0.5f); // Малиновый цвет с прозрачностью 0.5
		}
		else if (SelectedBranch->Show.Contains(BranchName.ToString()))
		{
			return FLinearColor(0.4f, 1.0f, 0.4f, 0.5f);
		}
	}

	return FLinearColor::Transparent;
}

void SBranchInfoWidget::Construct(const FArguments& InArgs)
{
	BranchName = InArgs._BranchName;
	Editor = InArgs._Editor;	

	DialoguePtr = Editor->GetDialogue();
	BranchRenamedHandle = DialoguePtr->OnBranchRenamed.AddRaw(this, &SBranchInfoWidget::OnBranchRenamed);

	ChildSlot
	[
		SNew(SBorder)
		// .Visibility(EVisibility::HitTestInvisible)
		.Padding(2.f)
		.BorderImage(FCoreStyle::Get().GetBrush("WhiteTexture"))
		.BorderBackgroundColor(this, &SBranchInfoWidget::GetBackgroundColor)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(150)
				[
					SAssignNew(BranchNameTextBox, SEditableTextBox)
					.Text(this, &SBranchInfoWidget::GetBranchNameAsText)
					.OnTextCommitted(this, &SBranchInfoWidget::OnBranchNameTextCommitted)
					.OnKeyDownHandler(this, &SBranchInfoWidget::OnBranchNameTextBoxKeyDown)
				]
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SAssignNew(BranchTextTextBox, SEditableTextBox)
				.Text_Raw(this, &SBranchInfoWidget::GetBranchText)
				.HintText_Raw(this, &SBranchInfoWidget::GetPlaceholderText)
				.OnTextCommitted(this, &SBranchInfoWidget::OnBranchTextTextCommitted)
				.OnKeyDownHandler(this, &SBranchInfoWidget::OnBranchTextTextBoxKeyDown)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(GrabButton, SButton)
				.ButtonStyle(FAppStyle::Get(), "FlatButton")
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.IsFocusable(false)
				.Visibility(EVisibility::HitTestInvisible)
				.Content()
				[
					SNew(SImage)
					.Image(FAppStyle::GetBrush("SoftwareCursor_Grab"))
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.ButtonStyle(FAppStyle::Get(), "FlatButton")
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.OnClicked(this, &SBranchInfoWidget::OnRemoveBranchButtonClicked)

				.Content()
				[
					SNew(SImage)
					.Image(FAppStyle::GetBrush("Cross"))
				]
			]
		]		
	];
}

SBranchInfoWidget::~SBranchInfoWidget()
{
	DialoguePtr->OnBranchRenamed.Remove(BranchRenamedHandle);
}

FName SBranchInfoWidget::GetBranchName() const
{
	return BranchName;
}

FText SBranchInfoWidget::GetBranchNameAsText() const
{
	return FText::FromName(BranchName);
}

void SBranchInfoWidget::SetEditableTextFocus(bool bIsTextFocused)
{
	if (bIsTextFocused)
	{
		FSlateApplication::Get().SetKeyboardFocus(BranchTextTextBox);
	}
	else
	{
		FSlateApplication::Get().SetKeyboardFocus(BranchNameTextBox);
	}
}

void SBranchInfoWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	
	const bool bNewFocus = BranchNameTextBox->HasKeyboardFocus() || BranchTextTextBox->HasKeyboardFocus();
	if (bNewFocus != bIsFocused)
	{
		SetIsFocused(bNewFocus);
	}
}

void SBranchInfoWidget::OnBranchRenamed(FName OldName, FName NewName)
{
	if (BranchName == OldName)
	{
		BranchName = NewName;
	}
}

FText SBranchInfoWidget::GetBranchText() const
{
	if (const auto BranchPtr = DialoguePtr->GetBranchPtr(BranchName))
	{
		return BranchPtr->Text;
	}
	return  FText::GetEmpty();
}

void SBranchInfoWidget::OnBranchNameTextCommitted(const FText& NewText, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
	{
		if (Editor)
		{
			const FName OldName = BranchName;
			const FName NewName = FName(*NewText.ToString());
			
			if (DialoguePtr && DialoguePtr->RenameBranch(OldName, NewName))
			{
				BranchName = NewName;
			}
			else
			{
				BranchNameTextBox->SetText(FText::FromName(OldName));
			}
		}
	}
}


void SBranchInfoWidget::OnBranchTextTextCommitted(const FText& NewText, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter || CommitType == ETextCommit::OnUserMovedFocus)
	{
		if (const auto BranchPtr = DialoguePtr->GetBranchPtr(BranchName))
		{
			if (!NewText.EqualTo(BranchPtr->Text))
			{
				const FScopedTransaction Transaction(LOCTEXT("SmartDialogueEditor_SetBranchText", "Set Branch Text"));
				DialoguePtr->Modify();
				BranchPtr->Text = NewText;
				DialoguePtr->PostEditChange();
			}
		}
	}
}

FReply SBranchInfoWidget::OnRemoveBranchButtonClicked()
{
	if (Editor)
	{
		Editor->RemoveBranch(this);
	}
	return FReply::Handled();
}

FReply SBranchInfoWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		FGeometry GrabButtonGeometry = FindChildGeometry(MyGeometry, GrabButton.ToSharedRef());

		if (GrabButtonGeometry.IsUnderLocation(MouseEvent.GetScreenSpacePosition()))
		{
			const TSharedRef<FBranchDragDropOperation> DragDropOp = FBranchDragDropOperation::New(SharedThis(this), Editor->GetBranchesListPanel());
			DraggedState(true);
			return FReply::Handled().BeginDragDrop(DragDropOp);
		}
	}

	return SCompoundWidget::OnMouseButtonDown(MyGeometry, MouseEvent);
}

TSharedPtr<SBranchInfoWidget> SBranchInfoWidget::GetParentBranchInfoWidget(const TSharedPtr<SWidget>& Widget)
{
	if (!Widget.IsValid())
	{
		return nullptr;
	}

	TSharedPtr<SBranchInfoWidget> BranchInfoWidget = StaticCastSharedPtr<SBranchInfoWidget>(Widget);

	if (BranchInfoWidget.IsValid())
	{
		return BranchInfoWidget;
	}

	return GetParentBranchInfoWidget(Widget->GetParentWidget());
}


void SBranchInfoWidget::SetIsFocused(bool bNewValue)
{
	bIsFocused = bNewValue;
	if (bNewValue)
	{
		Editor->SetSelectedBranchName(BranchName);
	}	
}

FText SBranchInfoWidget::GetPlaceholderText() const
{
	if (const auto BranchPtr = DialoguePtr->GetBranchPtr(BranchName))
	{
		if (BranchPtr->Phrases.Num() > 0)
		{
			return BranchPtr->Phrases[0].Text;
		}
	}
	return FText::GetEmpty();
}

FReply SBranchInfoWidget::OnBranchNameTextBoxKeyDown(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent)
{
	if (KeyEvent.GetKey() == EKeys::Up || KeyEvent.GetKey() == EKeys::Down)
	{
		MoveFocusToNextTextBox(KeyEvent.GetKey() == EKeys::Up ? -1 : 1);
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply SBranchInfoWidget::OnBranchTextTextBoxKeyDown(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent)
{
	if (KeyEvent.GetKey() == EKeys::Up || KeyEvent.GetKey() == EKeys::Down)
	{
		MoveFocusToNextTextBox(KeyEvent.GetKey() == EKeys::Up ? -1 : 1);
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void SBranchInfoWidget::MoveFocusToNextTextBox(int32 Direction)
{
	if (Direction == 1) // Moving Down
	{
		Editor->GetBranchesListPanel()->FocusNextBranchWidget(SharedThis(this), BranchTextTextBox->HasKeyboardFocus());
	}
	else if (Direction == -1) // Moving Up
	{
		Editor->GetBranchesListPanel()->FocusPreviousBranchWidget(SharedThis(this), BranchTextTextBox->HasKeyboardFocus());
	}
}

void SBranchInfoWidget::UnderDragState(bool bIsUnderDrag)
{
	bUnderDrag = bIsUnderDrag;
}

void SBranchInfoWidget::DraggedState(bool bIsDragged)
{
	bDragged = bIsDragged;
}

#undef LOCTEXT_NAMESPACE
