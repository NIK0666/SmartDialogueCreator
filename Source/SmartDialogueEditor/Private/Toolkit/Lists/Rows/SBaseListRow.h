// SBaseListRow.h

#pragma once

#include "CoreMinimal.h"
#include "ListItemData.h"
#include "Widgets/SCompoundWidget.h"

class FSmartDialogueEditor;

class SBaseListRow : public SCompoundWidget
{
public:	
	SLATE_BEGIN_ARGS(SBaseListRow) {}
	SLATE_ARGUMENT(FListItemData, Item)
	SLATE_ARGUMENT(TSharedPtr<FSmartDialogueEditor>, Editor)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FListItemData& GetItemData() {return Item;}
	
protected:
	virtual TSharedRef<SWidget> GetContent();
	
	FListItemData Item;
	TSharedPtr<FSmartDialogueEditor> Editor;

	virtual FReply RemoveItem();
};