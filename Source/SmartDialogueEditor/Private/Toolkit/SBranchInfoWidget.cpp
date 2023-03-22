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
	BranchPtr = InArgs._BranchPtr;
	Editor = InArgs._Editor;

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
				.Text(FText::FromName(BranchPtr->Name))
				.OnTextCommitted(this, &SBranchInfoWidget::OnBranchNameTextCommitted)
			]
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SAssignNew(BranchTextTextBox, SEditableTextBox)
			.Text(BranchPtr->Text)
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
		FName OldName = BranchPtr->Name;
		FName NewName = FName(*NewText.ToString());

		if (Editor.IsValid())
		{
			if (Editor.Pin()->GetDialogue() && Editor.Pin()->GetDialogue()->RenameBranch(OldName, NewName))
			{
				BranchPtr->Name = NewName;
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
		if (BranchPtr.IsValid())
		{
			BranchPtr->Text = NewText;
		}
	}
}
