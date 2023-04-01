// SBranchesWidget.h

#pragma once

#include "CoreMinimal.h"
#include "SmartDialogueData.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

class FSmartDialogueEditor;
class SBranchInfoWidget;
class USmartDialogue;

class SBranchesWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBranchesWidget) {}
	SLATE_ARGUMENT(FSmartDialogueEditor*, SmartDialogueEditor)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	// Обновляет список веток диалога
	void UpdateBranchesList();
	void RemoveRow(SBranchInfoWidget* BranchInfoWidget);

protected:
	// Добавляет новый элемент в список веток и обновляет его отображение
	void BranchItemAdded(FSmartDialogueBranch& NewBranch);

private:
	// Генерирует строку SBranchInfoWidget для SListView
	TSharedRef<ITableRow> GenerateBranchInfoWidgetRow(TSharedPtr<SBranchInfoWidget> InItem, const TSharedRef<STableViewBase>& OwnerTable);

	// Ссылка на редактор SmartDialogue
	FSmartDialogueEditor* SmartDialogueEditor;

	// Список виджетов SBranchInfoWidget
	TArray<TSharedPtr<SBranchInfoWidget>> BranchesInfoWidgets;

	// Виджет SListView для отображения веток диалога
	TSharedPtr<SListView<TSharedPtr<SBranchInfoWidget>>> BranchesList;
};
