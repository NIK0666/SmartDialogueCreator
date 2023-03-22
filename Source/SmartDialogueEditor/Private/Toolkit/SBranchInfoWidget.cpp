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
	Branch = InArgs._Branch;
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
				.Text(FText::FromName(Branch.Name))
				.OnTextCommitted(this, &SBranchInfoWidget::OnBranchNameTextCommitted)
			]
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SAssignNew(BranchTextTextBox, SEditableTextBox)
			.Text(Branch.Text)
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
		FName OldName = Branch.Name;
		FName NewName = FName(*NewText.ToString());

		if (Editor.IsValid())
		{
			if (Editor.Pin()->GetDialogue() && Editor.Pin()->GetDialogue()->RenameBranch(OldName, NewName))
			{
				Branch.Name = NewName;
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
		if (Editor.IsValid())
		{
			
		}
	}
}
