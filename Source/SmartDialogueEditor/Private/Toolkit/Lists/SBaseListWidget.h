// SBaseListWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Rows/ListItemData.h"
#include "Widgets/SCompoundWidget.h"

class FSmartDialogueEditor;
class SBaseListRow;

class SBaseListWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBaseListWidget)
		: _Title(FText::GetEmpty())
	{}
	SLATE_ARGUMENT(FText, Title)
	SLATE_ARGUMENT(FSmartDialogueEditor*, Editor)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void UpdateData(const TArray<FListItemData>& NewData);
	virtual void RemoveItem(const FListItemData& ItemToRemove);

	FReply OnAddButtonClicked();
	TSharedRef<SWidget> GenerateStringItemWidget(TSharedPtr<FString> InItem);
	void OnComboBoxSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	
protected:
	virtual TSharedRef<SWidget> GetItemContent(const FListItemData& Item);
	virtual TArray<TSharedPtr<FString>> GetAllStrings();;
	TSharedRef<SWidget> CreateMenuContent();
	virtual FReply OnContextMenuItemClicked(const FString& Item);
	virtual void OnSelected(const FListItemData& SelectedItem);

	FText Title;
	FSmartDialogueEditor* Editor = nullptr;
	TArray<FListItemData> Data;

	TSharedPtr<SVerticalBox> ListContainer;
	TSharedPtr<SMenuAnchor> ContextMenuAnchor;
};
