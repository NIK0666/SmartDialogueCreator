// SBranchListRow.h

#pragma once

#include "CoreMinimal.h"
#include "SBaseListRow.h"

class SBranchListRow : public SBaseListRow
{
public:
	SLATE_BEGIN_ARGS(SBranchListRow) {}
		SLATE_ARGUMENT(FListItemData, Item)
		SLATE_ARGUMENT(bool, bIsShowed)
		SLATE_ARGUMENT(FSmartDialogueEditor*, Editor)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	virtual TSharedRef<SWidget> GetContent() override;
	virtual FReply RemoveItem() override;
private:	
	bool bIsShowed = false; 
};