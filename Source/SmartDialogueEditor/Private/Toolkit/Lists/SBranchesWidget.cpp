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

	SmartDialogueEditor->OnBranchItemAdded.BindSP(this, &SBranchesWidget::BranchItemAdded);
	
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
	if (!SmartDialogueEditor)
	{
		return;
	}

	BranchesInfoWidgets.Empty();
	USmartDialogue* Dialogue = SmartDialogueEditor->GetDialogue();

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

void SBranchesWidget::RemoveRow(SBranchInfoWidget* BranchInfoWidget)
{
	if (!BranchInfoWidget)
	{
		return;
	}

	TSharedPtr<SBranchInfoWidget> BranchInfoWidgetToRemove;
    
	// Найти TSharedPtr для BranchInfoWidget, которое нужно удалить
	for (const auto& BranchInfo : BranchesInfoWidgets)
	{
		if (BranchInfo.Get() == BranchInfoWidget)
		{
			BranchInfoWidgetToRemove = BranchInfo;
			break;
		}
	}

	if (BranchInfoWidgetToRemove.IsValid())
	{
		// Удалить найденный элемент из TArray
		BranchesInfoWidgets.Remove(BranchInfoWidgetToRemove);

		// Обновить SListView
		BranchesList->RequestListRefresh();
	}
}

void SBranchesWidget::FocusNextBranchWidget(TSharedPtr<SBranchInfoWidget> CurrentWidget, bool bIsTextFocused)
{
	int32 Index = BranchesInfoWidgets.IndexOfByPredicate([CurrentWidget](const TSharedPtr<SBranchInfoWidget>& Widget) {
		return Widget == CurrentWidget;
	});

	if (Index != INDEX_NONE && Index + 1 < BranchesInfoWidgets.Num())
	{
		BranchesInfoWidgets[Index + 1]->SetEditableTextFocus(bIsTextFocused);
	}
}

void SBranchesWidget::FocusPreviousBranchWidget(TSharedPtr<SBranchInfoWidget> CurrentWidget, bool bIsTextFocused)
{
	int32 Index = BranchesInfoWidgets.IndexOfByPredicate([CurrentWidget](const TSharedPtr<SBranchInfoWidget>& Widget) {
		return Widget == CurrentWidget;
	});

	if (Index != INDEX_NONE && Index - 1 >= 0)
	{
		return BranchesInfoWidgets[Index - 1]->SetEditableTextFocus(bIsTextFocused);
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
