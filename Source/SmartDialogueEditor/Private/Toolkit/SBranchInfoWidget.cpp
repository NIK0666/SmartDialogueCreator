// SBranchInfoWidget.cpp

#include "SBranchInfoWidget.h"

#include "FSmartDialogueEditor.h"
#include "SmartDialogue.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "EditorStyle/Public/EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "SmartDialogueEditor"

FSlateColor SBranchInfoWidget::GetBackgroundColor() const
{
	if (Editor.Pin()->GetSelectedBranchName() == BranchName)
	{
		return FLinearColor(0.25f, 0.88f, 0.82f, 0.5f);
	}

	return FLinearColor::Transparent;
}

void SBranchInfoWidget::Construct(const FArguments& InArgs)
{
	BranchName = InArgs._BranchName;
	Editor = InArgs._Editor;	

	DialoguePtr = Editor.Pin()->GetDialogue();
	
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
				]
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SAssignNew(BranchTextTextBox, SEditableTextBox)
				.Text_Raw(this, &SBranchInfoWidget::GetBranchText)
				.OnTextCommitted(this, &SBranchInfoWidget::OnBranchTextTextCommitted)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(GrabButton, SButton)
				.ButtonStyle(FEditorStyle::Get(), "FlatButton")
				.ContentPadding(FMargin(1.0f, 1.0f, 1.0f, 1.0f))
				.OnPressed(this, &SBranchInfoWidget::OnGrabButtonPressed)
				.OnReleased(this, &SBranchInfoWidget::OnGrabButtonReleased)
				[
					SNew(SImage)
					.Image(FAppStyle::GetBrush("SoftwareCursor_Grab"))
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.ButtonStyle(FEditorStyle::Get(), "FlatButton")
				.ContentPadding(FMargin(1.0f, 1.0f, 1.0f, 1.0f))
				[
					SNew(SImage)
					.Image(FAppStyle::GetBrush("Icons.Delete")) 
				]
			]
		]		
	];
}

void SBranchInfoWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	
	// TSharedPtr<SWidget> FocusedWidget = FSlateApplication::Get().GetKeyboardFocusedWidget();
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

		if (Editor.IsValid())
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

void SBranchInfoWidget::OnGrabButtonPressed()
{
	TSharedPtr<SWidget> ButtonContent = GrabButton->GetContent();
	TSharedPtr<SImage> ButtonImage = StaticCastSharedPtr<SImage>(ButtonContent);

	ButtonImage->SetImage(FAppStyle::GetBrush("SoftwareCursor_UpDown"));
}

void SBranchInfoWidget::OnGrabButtonReleased()
{
	TSharedPtr<SWidget> ButtonContent = GrabButton->GetContent();
	TSharedPtr<SImage> ButtonImage = StaticCastSharedPtr<SImage>(ButtonContent);

	ButtonImage->SetImage(FAppStyle::GetBrush("SoftwareCursor_Grab"));
}

void SBranchInfoWidget::SetIsFocused(bool bNewValue)
{
	bIsFocused = bNewValue;
	if (bNewValue)
	{
		Editor.Pin()->SetSelectedBranchName(BranchName);
	}	
}
#undef LOCTEXT_NAMESPACE
