// SBranchListItemWidget.h

#pragma once

#include "CoreMinimal.h"
#include "SBaseListItemWidget.h"

class SBranchListItemWidget : public SBaseListItemWidget
{
public:
	SLATE_BEGIN_ARGS(SBranchListItemWidget) {}
		SLATE_ARGUMENT(FListItemData, Item)
		SLATE_EVENT(FOnClicked, OnChangeClicked)
		SLATE_ARGUMENT(bool, bIsShowed)
		SLATE_ARGUMENT(TWeakPtr<FSmartDialogueEditor>, Editor)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	virtual TSharedRef<SWidget> GetContent() override;
	virtual FReply RemoveItem() override;

private:	
	FReply OnChangeButtonClicked();
	FOnClicked OnChangeClicked;
	bool bIsShowed = false; 
};