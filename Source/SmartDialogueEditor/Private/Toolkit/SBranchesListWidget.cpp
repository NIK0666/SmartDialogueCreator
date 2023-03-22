// 


#include "SBranchesListWidget.h"

#include "SBranchInfoWidget.h"
#include "SmartDialogue.h"
#include "SmartDialogueEditor/Private/Toolkit/FSmartDialogueEditor.h"

void SBranchesListWidget::Construct(const FArguments& InArgs)
{
	SmartDialogueEditor = InArgs._SmartDialogueEditor;

	SmartDialogueEditor->OnBranchItemAdded.BindSP(this, &SBranchesListWidget::BranchItemAdded);
	SmartDialogueEditor->OnBranchItemDeleted.BindSP(this, &SBranchesListWidget::BranchItemDeleted);
	
	// Обновляем список веток при создании виджета
	UpdateBranchesList();
}

void SBranchesListWidget::UpdateBranchesList()
{
	// Здесь должен быть код очистки списка и генерации его из массива Branches
	for (auto L_Branch : SmartDialogueEditor.Get()->GetDialogue()->Branches)
	{
		// 
	}
}

void SBranchesListWidget::BranchItemAdded(FSmartDialogueBranch& AddedBranch)
{
	// Здесь нужно добавить новый элемент SBranchInfoWidget в элемент списка
}

void SBranchesListWidget::BranchItemDeleted(FSmartDialogueBranch& DeletedBranch)
{
	// Здесь нужно удалить элемент SBranchInfoWidget из списка
}
