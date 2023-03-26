// SBaseListItemWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SBaseListItemWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBaseListItemWidget) {}
		SLATE_ARGUMENT(FString, Item)
		SLATE_EVENT(FOnClicked, OnRemoveClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
protected:
	virtual TSharedRef<SWidget> GetContent();
	
	FString Item;
private:
	FReply OnRemoveButtonClicked();
	FOnClicked OnRemoveClicked;
};