// 


#include "SBranchesListWidget.h"

#include "SBranchInfoWidget.h"
#include "SmartDialogue.h"
#include "SmartDialogueEditor/Private/Toolkit/FSmartDialogueEditor.h"

void SBranchesListWidget::Construct(const FArguments& InArgs)
{
	SmartDialogueEditor = InArgs._SmartDialogueEditor;

	// Подписываемся на делегат
	SmartDialogueEditor->OnBranchListModified.BindSP(this, &SBranchesListWidget::UpdateBranchesList);

	// Обновляем список веток при создании виджета
	UpdateBranchesList();
}

void SBranchesListWidget::UpdateBranchesList()
{
	DialogueBranchWidgets.Reset();
	
	for (FSmartDialogueBranch& Branch : SmartDialogueEditor->GetDialogue()->Branches)
	{
		DialogueBranchWidgets.Add(SNew(SBranchInfoWidget)
			.Branch(Branch)
			.Editor(SmartDialogueEditor));
	}
}
