// SBaseListItemWidget.h

#pragma once

#include "CoreMinimal.h"
#include "ListItemData.h"
#include "Widgets/SCompoundWidget.h"

class FSmartDialogueEditor;

class SBaseListItemWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBaseListItemWidget) {}
		SLATE_ARGUMENT(FListItemData, Item)
		SLATE_ARGUMENT(TWeakPtr<FSmartDialogueEditor>, Editor)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
protected:
	virtual TSharedRef<SWidget> GetContent();
	
	FListItemData Item;
	TWeakPtr<FSmartDialogueEditor> Editor;

	virtual FReply RemoveItem();
};