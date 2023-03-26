// SBranchesListWidget.h
#pragma once

#include "CoreMinimal.h"
#include "SVerticalListWidget.h"

class SBranchesListWidget : public SVerticalListWidget
{
	
public:	
	void Construct(const FArguments& InArgs);

protected:
	TSharedRef<SWidget> GetItemContent(const FString& Item) override;
	void ShowSelectionMenu();
	FReply OnChangeButtonClicked();
};
