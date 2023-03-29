// SBranchesWidget.cpp


#include "SBranchesWidget.h"

#include "Toolkit/FSmartDialogueEditor.h"
#include "SmartDialogue.h"
#include "Toolkit/SBranchInfoWidget.h"
#include "SmartDialogueEditor.h"
#include "Widgets/Layout/SScrollBox.h"

void SBranchesWidget::Construct(const FArguments& InArgs)
{
	SmartDialogueEditor = InArgs._SmartDialogueEditor;

	UpdateBranchesList();

	SmartDialogueEditor.Get()->OnBranchItemAdded.BindSP(this, &SBranchesWidget::BranchItemAdded);
	SmartDialogueEditor.Get()->OnBranchItemDeleted.BindSP(this, &SBranchesWidget::BranchItemDeleted);
	
	ChildSlot
	[
		SNew(SScrollBox)
		.Orientation(Orient_Vertical)
		+ SScrollBox::Slot()
		[
			// Создаем вертикальный список
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.FillHeight(1.f)
			[
				// Заполняем список элементами SBranchInfoWidget
				SAssignNew(BranchesList, SListView<TSharedPtr<SBranchInfoWidget>>)
				.SelectionMode(ESelectionMode::None)
				.ListItemsSource(&BranchesInfoWidgets)
				.OnGenerateRow(this, &SBranchesWidget::GenerateBranchInfoWidgetRow)
			]
		]
	];
}

TSharedRef<ITableRow> SBranchesWidget::GenerateBranchInfoWidgetRow(TSharedPtr<SBranchInfoWidget> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<SBranchInfoWidget>>, OwnerTable)
		[
			InItem.ToSharedRef()
		];
}

void SBranchesWidget::UpdateBranchesList()
{
	if (!SmartDialogueEditor.IsValid())
	{
		return;
	}

	BranchesInfoWidgets.Empty();
	USmartDialogue* Dialogue = SmartDialogueEditor.Get()->GetDialogue();

	if (Dialogue)
	{
		for (const TPair<FName, FSmartDialogueBranch>& Pair : Dialogue->GetBranches())
		{
			BranchesInfoWidgets.Add(
				SNew(SBranchInfoWidget)
				.BranchName(Pair.Key)
				.Editor(SmartDialogueEditor)
			);
		}
	}

	if (BranchesList.IsValid())
	{
		BranchesList->RequestListRefresh();
	}
}

void SBranchesWidget::BranchItemAdded(FSmartDialogueBranch& AddedBranch) 
{
	TSharedPtr<SBranchInfoWidget> NewBranchInfoWidget = SNew(SBranchInfoWidget)
			.BranchName(AddedBranch.Name)
			.Editor(SmartDialogueEditor);

	BranchesInfoWidgets.Add(NewBranchInfoWidget);

	if (BranchesList.IsValid())
	{
		BranchesList->RequestListRefresh();
	}
	
}

void SBranchesWidget::BranchItemDeleted(FSmartDialogueBranch& DeletedBranch)
{
	// Здесь нужно удалить элемент SBranchInfoWidget из списка
}
