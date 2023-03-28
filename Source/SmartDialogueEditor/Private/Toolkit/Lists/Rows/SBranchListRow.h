// SBranchListRow.h

#pragma once

#include "CoreMinimal.h"
#include "SBaseListRow.h"

class SBranchListRow : public SBaseListRow
{
public:
	SLATE_BEGIN_ARGS(SBranchListRow) {}
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