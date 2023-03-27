// SBaseListWidget.h

#pragma once

#include "CoreMinimal.h"
#include "ListItemData.h"
#include "Widgets/SCompoundWidget.h"

class FSmartDialogueEditor;

class SBaseListWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBaseListWidget)
		: _Title(FText::GetEmpty())
	{}
	SLATE_ARGUMENT(FText, Title)
	SLATE_ARGUMENT(TWeakPtr<FSmartDialogueEditor>, Editor)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void UpdateData(const TArray<FListItemData>& NewData);
	
	void OnRemoveButtonClicked(const int32 IndexToRemove);
	FReply OnAddButtonClicked();
	TSharedRef<SWidget> GenerateStringItemWidget(TSharedPtr<FString> InItem);
	void OnComboBoxSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	
protected:
	virtual TSharedRef<SWidget> GetItemContent(const FListItemData& Item);
	virtual TArray<FString> GetAllStrings();
	TSharedRef<SWidget> CreateMenuContent();
	FReply OnMenuItemClicked(const FString& Item);
	virtual void OnSelected(const FListItemData& SelectedItem);
	
	FText Title;
	TWeakPtr<FSmartDialogueEditor> Editor;

private:
	TSharedPtr<SVerticalBox> ListContainer;
	TSharedPtr<SMenuAnchor> ContextMenuAnchor;
	TArray<FListItemData> Data;
};
