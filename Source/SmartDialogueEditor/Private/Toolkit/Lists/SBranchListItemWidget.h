// SBranchListItemWidget.h

#pragma once

#include "CoreMinimal.h"
#include "SBaseListItemWidget.h"

class SBranchListItemWidget : public SBaseListItemWidget
{
public:
	SLATE_BEGIN_ARGS(SBranchListItemWidget) {}
		SLATE_ARGUMENT(FString, Item)
		SLATE_EVENT(FOnClicked, OnChangeClicked)
		SLATE_EVENT(FOnClicked, OnRemoveClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	virtual TSharedRef<SWidget> GetContent() override;

private:	
	FReply OnChangeButtonClicked();
	FOnClicked OnChangeClicked;
};