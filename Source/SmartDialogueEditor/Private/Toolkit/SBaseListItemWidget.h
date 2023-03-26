// SBaseListItemWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SBaseListItemWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBaseListItemWidget) {}
	SLATE_ARGUMENT(FString, Item)
	SLATE_EVENT(FSimpleDelegate, OnChangeClicked)
	SLATE_EVENT(FSimpleDelegate, OnRemoveClicked)
SLATE_END_ARGS()

void Construct(const FArguments& InArgs);

private:
	FReply OnChangeButtonClicked();
	FReply OnRemoveButtonClicked();
	
	FString Item;
	FSimpleDelegate OnChangeClicked;
	FSimpleDelegate OnRemoveClicked;
};