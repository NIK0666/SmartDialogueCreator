// SBranchesListWidget.cpp


#include "SBranchesListWidget.h"

#include "FSmartDialogueEditor.h"
#include "SmartDialogue.h"
#include "SBranchInfoWidget.h"
#include "SmartDialogueEditor.h"

void SBranchesListWidget::Construct(const FArguments& InArgs)
{
	SmartDialogueEditor = InArgs._SmartDialogueEditor;

	UpdateBranchesList();

	SmartDialogueEditor.Pin()->OnBranchItemAdded.BindSP(this, &SBranchesListWidget::BranchItemAdded);
	SmartDialogueEditor.Pin()->OnBranchItemDeleted.BindSP(this, &SBranchesListWidget::BranchItemDeleted);
	
	ChildSlot
	[
		// Создаем вертикальный список
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			// Заполняем список элементами SBranchInfoWidget
			SAssignNew(BranchesList, SListView<TSharedPtr<SBranchInfoWidget>>)
			.SelectionMode(ESelectionMode::None)
			.ListItemsSource(&BranchesInfoWidgets)
			.OnGenerateRow(this, &SBranchesListWidget::GenerateBranchInfoWidgetRow)
		]
	];
}

TSharedRef<ITableRow> SBranchesListWidget::GenerateBranchInfoWidgetRow(TSharedPtr<SBranchInfoWidget> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<SBranchInfoWidget>>, OwnerTable)
		[
			InItem.ToSharedRef()
		];
}

void SBranchesListWidget::UpdateBranchesList()
{
	if (!SmartDialogueEditor.IsValid())
	{
		return;
	}

	BranchesInfoWidgets.Empty();
	USmartDialogue* Dialogue = SmartDialogueEditor.Pin()->GetDialogue();

	if (Dialogue)
	{
		for (auto& Branch : Dialogue->Branches)
		{
			BranchesInfoWidgets.Add(
				SNew(SBranchInfoWidget)
				.Branch(Branch)
				.Editor(SmartDialogueEditor)
			);
		}
	}

	if (BranchesList.IsValid())
	{
		BranchesList->RequestListRefresh();
	}
}

void SBranchesListWidget::BranchItemAdded(FSmartDialogueBranch& AddedBranch)
{
	TSharedPtr<SBranchInfoWidget> NewBranchInfoWidget = SNew(SBranchInfoWidget)
			.Branch(AddedBranch)
			.Editor(SmartDialogueEditor);

	BranchesInfoWidgets.Add(NewBranchInfoWidget);

	if (BranchesList.IsValid())
	{
		BranchesList->RequestListRefresh();
	}
}

void SBranchesListWidget::BranchItemDeleted(FSmartDialogueBranch& DeletedBranch)
{
	// Здесь нужно удалить элемент SBranchInfoWidget из списка
}
