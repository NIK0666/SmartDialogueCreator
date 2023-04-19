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

	SmartDialogueEditor->OnBranchItemAdded.AddSP(this, &SBranchesWidget::BranchItemAdded);
	
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

	TArray<FName> CurrentBranchNames;
	for (const auto& Widget : BranchesInfoWidgets)
	{
		CurrentBranchNames.Add(Widget->GetBranchName());
	}

	if (const USmartDialogue* Dialogue = SmartDialogueEditor->GetDialogue())
	{
		TArray<FName> NewBranchNames;
		for (const TPair<FName, FSmartDialogueBranch>& Pair : Dialogue->GetBranches())
		{
			NewBranchNames.Add(Pair.Key);

			if (!CurrentBranchNames.Contains(Pair.Key))
			{
				BranchesInfoWidgets.Add(
					SNew(SBranchInfoWidget)
					.BranchName(Pair.Key)
					.Editor(SmartDialogueEditor)
				);
			}
		}

		// Remove extra widgets
		for (int32 i = BranchesInfoWidgets.Num() - 1; i >= 0; --i)
		{
			if (!NewBranchNames.Contains(BranchesInfoWidgets[i]->GetBranchName()))
			{
				BranchesInfoWidgets.RemoveAt(i);
			}
		}

		// Reorder widgets according to the new order
		BranchesInfoWidgets.Sort([Dialogue](const TSharedPtr<SBranchInfoWidget>& A, const TSharedPtr<SBranchInfoWidget>& B)
		{
			const auto& Branches = Dialogue->GetBranches();
			int32 IndexA = INDEX_NONE;
			int32 IndexB = INDEX_NONE;
			int32 CurrentIndex = 0;

			for (const auto& Branch : Branches)
			{
				if (Branch.Key == A->GetBranchName())
				{
					IndexA = CurrentIndex;
				}

				if (Branch.Key == B->GetBranchName())
				{
					IndexB = CurrentIndex;
				}

				++CurrentIndex;

				// If both indices are found, no need to continue the loop
				if (IndexA != INDEX_NONE && IndexB != INDEX_NONE)
				{
					break;
				}
			}

			return IndexA < IndexB;
		});
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

void SBranchesWidget::DragEndBranchInfo(TSharedPtr<SBranchInfoWidget> DraggedWidget, const FVector2D& MousePosition)
{
	DraggedWidget->DraggedState(false);

	if (CurrentlyHighlightedWidget.IsValid())
	{
		CurrentlyHighlightedWidget->UnderDragState(false);
		
		SmartDialogueEditor->GetDialogue()->MoveBranch(DraggedWidget->GetBranchName(), CurrentlyHighlightedWidget->GetBranchName());
		UpdateBranchesList();
	}
}

void SBranchesWidget::DragProcessBranchInfo(TSharedPtr<SBranchInfoWidget> DraggedWidget, const FVector2D& MousePosition)
{
	TSharedPtr<SBranchInfoWidget> NewHighlightedWidget = nullptr;

	for (const TSharedPtr<SBranchInfoWidget>& BranchInfoWidget : BranchesInfoWidgets)
	{
		if (BranchInfoWidget == DraggedWidget) // Пропустите DraggedWidget в списке
			{
			continue;
			}

		FGeometry WidgetGeometry = BranchInfoWidget->GetCachedGeometry();
		if (WidgetGeometry.IsUnderLocation(MousePosition))
		{
			NewHighlightedWidget = BranchInfoWidget;
			break;
		}
	}

	if (NewHighlightedWidget != CurrentlyHighlightedWidget)
	{
		if (CurrentlyHighlightedWidget.IsValid())
		{
			CurrentlyHighlightedWidget->UnderDragState(false);
		}

		CurrentlyHighlightedWidget = NewHighlightedWidget;

		if (CurrentlyHighlightedWidget.IsValid())
		{
			CurrentlyHighlightedWidget->UnderDragState(true);
		}
	}
}

