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

	ChildSlot
	[
		SNew(SListView<TSharedRef<SBranchInfoWidget>>)
			.SelectionMode(ESelectionMode::None)
			.ListItemsSource(&DialogueBranchWidgets)
			.OnGenerateRow(this, &SBranchesListWidget::OnGenerateRowForBranchList)
	];
	
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

TSharedRef<ITableRow> SBranchesListWidget::OnGenerateRowForBranchList(TSharedRef<SBranchInfoWidget> InWidget, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedRef<SBranchInfoWidget>>, OwnerTable)
		.Content()
		[
			InWidget
		];
}
