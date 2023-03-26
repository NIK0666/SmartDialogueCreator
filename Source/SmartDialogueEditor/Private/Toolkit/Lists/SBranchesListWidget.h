// SBranchesListWidget.h
#pragma once

#include "CoreMinimal.h"
#include "SBaseListWidget.h"

class SBranchesListWidget : public SBaseListWidget
{
	
public:
	SLATE_BEGIN_ARGS(SBranchesListWidget) {}
	SLATE_ARGUMENT(FText, Title)
	SLATE_ARGUMENT(TWeakPtr<FSmartDialogueEditor>, Editor)
	SLATE_ARGUMENT(bool, bIsShowed)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

protected:
	TSharedRef<SWidget> GetItemContent(const FListItemData& Item) override;
	void ShowSelectionMenu();
	FReply OnChangeButtonClicked();
	bool bIsShowed;
};
