// SBranchInfoWidget.cpp

#include "SBranchInfoWidget.h"

#include "FSmartDialogueEditor.h"
#include "SmartDialogue.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "EditorStyle/Public/EditorStyleSet.h"

void SBranchInfoWidget::Construct(const FArguments& InArgs)
{
	BranchName = InArgs._BranchName;
	Editor = InArgs._Editor;

	DialoguePtr = Editor.Pin()->GetDialogue();
	BranchPtr = DialoguePtr->GetBranchPtr(BranchName);

	ChildSlot
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
			.Text(BranchPtr ? BranchPtr->Text : FText::GetEmpty())
			.OnTextCommitted(this, &SBranchInfoWidget::OnBranchTextTextCommitted)
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.ButtonStyle(FEditorStyle::Get(), "FlatButton")
			.ContentPadding(FMargin(1.0f, 1.0f, 1.0f, 1.0f))
			[
				SNew(SImage)
				.Image(FEditorStyle::GetBrush("Cross"))
			]
			// ...
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.ButtonStyle(FEditorStyle::Get(), "FlatButton")
			.ContentPadding(FMargin(1.0f, 1.0f, 1.0f, 1.0f))
			[
				SNew(SImage)
				.Image(FEditorStyle::GetBrush("Cross"))
			]
			// ...
		]
	];
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
		if (!NewText.EqualTo(BranchPtr->Text))
		{
			BranchPtr->Text = NewText;
		}		
	}
}
