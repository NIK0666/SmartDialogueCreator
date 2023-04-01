// SBranchPhrasesWidget.cpp

#include "SBranchPhrasesWidget.h"

#include "EditorStyleSet.h"
#include "FSmartDialogueEditor.h"
#include "SmartDialogue.h"
#include "Lists/Rows/SPhraseListRow.h"
#include "Widgets/Layout/SScrollBox.h"

#define LOCTEXT_NAMESPACE "SmartDialogueEditor"

void SBranchPhrasesWidget::Construct(const FArguments& InArgs)
{
	SmartDialogueEditor = InArgs._SmartDialogueEditor;

	SmartDialogueEditor->OnBranchSelected.AddSP(this, &SBranchPhrasesWidget::OnBranchSelected);
	SmartDialogueEditor->OnResetSelectedBranch.AddSP(this, &SBranchPhrasesWidget::OnResetSelectedBranch);

	UpdatePhrases();

	ChildSlot
	[    SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(FMargin(0.0f, 4.0f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(4.f, 0.f)
			.VAlign(VAlign_Center)
			[
				SAssignNew(AddNewPhraseButton, SButton)
				.HAlign(HAlign_Left)
				.Text(LOCTEXT("AddNewPhrase", "Add New Phrase"))
				.OnClicked_Lambda([this]() {
					if (SmartDialogueEditor->GetSelectedBranch())
					{
						FSmartDialoguePhrase NpcPhrase;
						NpcPhrase.NPC = SmartDialogueEditor->GetDialogue()->GetCharacter();
						SmartDialogueEditor->GetSelectedBranch()->Phrases.Add(NpcPhrase);

						UpdatePhrases();
					}
					return FReply::Handled();
				})
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(4.f, 0.f)
			.VAlign(VAlign_Center)
			[
				SAssignNew(AddFirstHeroPhraseButton, SButton)
				.HAlign(HAlign_Left)
				.Text(LOCTEXT("AddFirstHeroPhrase", "Add First Hero Phrase"))
				.OnClicked_Lambda([this]() {
					if (SmartDialogueEditor->GetSelectedBranch())
					{
						FSmartDialoguePhrase HeroPhrase;
						HeroPhrase.NPC = SmartDialogueEditor->GetDialogueConfig()->GetHero();
						HeroPhrase.Text = SmartDialogueEditor->GetSelectedBranch()->Text;
						
						SmartDialogueEditor->GetSelectedBranch()->Phrases.Add(HeroPhrase);

						UpdatePhrases();
					}
					return FReply::Handled();
				})
			]
		]
		+ SVerticalBox::Slot()
		.FillHeight(1.f)
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			[
				PhrasesVBox.ToSharedRef()
			]
		]
	];
}
void SBranchPhrasesWidget::OnBranchSelected(FSmartDialogueBranch& SmartDialogueBranch)
{
	UpdatePhrases();
}

void SBranchPhrasesWidget::OnResetSelectedBranch()
{
	PhrasesVBox->ClearChildren();	
}

void SBranchPhrasesWidget::UpdatePhrases()
{
	if (!PhrasesVBox.IsValid())
	{
		PhrasesVBox = SNew(SVerticalBox);
	}
	else
	{
		PhrasesVBox->ClearChildren();
	}

	if (SmartDialogueEditor->GetSelectedBranch() == nullptr)
	{
		return;
	}

	if (AddFirstHeroPhraseButton.IsValid())
	{
		AddFirstHeroPhraseButton->SetVisibility(SmartDialogueEditor->GetSelectedBranch()->Phrases.IsEmpty() ? EVisibility::Visible : EVisibility::Collapsed);
	}

	for (auto& Phrase : SmartDialogueEditor->GetSelectedBranch()->Phrases)
	{
		PhrasesVBox->AddSlot()
		           .AutoHeight()
		           .Padding(FMargin(0.0f, 2.0f))
			[
				SNew(SPhraseListRow)
				.SmartDialoguePhrasePtr(&Phrase)
				.SmartDialogueEditor(SmartDialogueEditor)
			];
	}
}

#undef LOCTEXT_NAMESPACE