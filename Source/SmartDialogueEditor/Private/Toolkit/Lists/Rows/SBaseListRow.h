// SBaseListRow.h

#pragma once

#include "CoreMinimal.h"
#include "ListItemData.h"
#include "Widgets/SCompoundWidget.h"

class FSmartDialogueEditor;

class SBaseListRow : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_OneParam(FOnRemoveItemRequested, const FListItemData&);

	
	SLATE_BEGIN_ARGS(SBaseListRow) {}
	SLATE_ARGUMENT(FListItemData, Item)
	SLATE_ARGUMENT(FSmartDialogueEditor*, Editor)
	SLATE_EVENT(FOnRemoveItemRequested, OnRemoveItemRequested)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FListItemData& GetItemData() {return Item;}
	
protected:
	virtual TSharedRef<SWidget> GetContent();
	virtual FReply RemoveItem();
	
	FListItemData Item;
	FSmartDialogueEditor* Editor = nullptr;
	FOnRemoveItemRequested OnRemoveItemRequested;

};