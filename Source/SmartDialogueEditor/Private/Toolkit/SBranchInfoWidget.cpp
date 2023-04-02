// SBranchInfoWidget.cpp

#include "SBranchInfoWidget.h"

#include "FSmartDialogueEditor.h"
#include "SmartDialogue.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "EditorStyle/Public/EditorStyleSet.h"
#include "Lists/SBranchesWidget.h"

#define LOCTEXT_NAMESPACE "SmartDialogueEditor"

FSlateColor SBranchInfoWidget::GetBackgroundColor() const
{

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
					.Text(FText::FromName(BranchName))
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
				.OnPressed(this, &SBranchInfoWidget::OnGrabButtonPressed)
				.OnReleased(this, &SBranchInfoWidget::OnGrabButtonReleased)
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

FName SBranchInfoWidget::GetBranchName() const
{
	return BranchName;
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
		FName OldName = BranchName;
		FName NewName = FName(*NewText.ToString());

		if (Editor)
		{
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


void SBranchInfoWidget::OnGrabButtonPressed()
{
	TSharedPtr<SWidget> ButtonContent = GrabButton->GetContent();
	TSharedPtr<SImage> ButtonImage = StaticCastSharedPtr<SImage>(ButtonContent);
}

void SBranchInfoWidget::OnGrabButtonReleased()
{
	TSharedPtr<SWidget> ButtonContent = GrabButton->GetContent();
	TSharedPtr<SImage> ButtonImage = StaticCastSharedPtr<SImage>(ButtonContent);
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

#undef LOCTEXT_NAMESPACE
