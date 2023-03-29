// SOperationsListWidget.h

#pragma once

#include "SBaseListWidget.h"

class SOperationsListWidget : public SBaseListWidget
{
public:
	SLATE_BEGIN_ARGS(SOperationsListWidget) {}
	SLATE_ARGUMENT(FText, Title)
	SLATE_ARGUMENT(TSharedPtr<FSmartDialogueEditor>, Editor)
	SLATE_ARGUMENT(bool, bIsExecution)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	virtual TSharedRef<SWidget> GetItemContent(const FListItemData& Item) override;
	virtual TArray<TSharedPtr<FString>> GetAllStrings() override;
	virtual FReply OnContextMenuItemClicked(const FString& Item) override;

	bool bIsExecution = false;
};