// SBranchesListWidget.h
#pragma once

#include "CoreMinimal.h"
#include "SBaseListWidget.h"

class SBranchesListWidget : public SBaseListWidget
{
	
public:
	SLATE_BEGIN_ARGS(SBranchesListWidget) {}
	SLATE_ARGUMENT(FText, Title)
	SLATE_ARGUMENT(FSmartDialogueEditor*, Editor)
	SLATE_ARGUMENT(bool, bIsShowed)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	TSharedRef<SWidget> GetItemContent(const FListItemData& Item) override;
	
	void OnBranchItemRemoved(FName& Name);

	virtual TArray<TSharedPtr<FString>> GetAllStrings() override;
	virtual FReply OnContextMenuItemClicked(const FString& Item) override;
	bool bIsShowed = false;
};