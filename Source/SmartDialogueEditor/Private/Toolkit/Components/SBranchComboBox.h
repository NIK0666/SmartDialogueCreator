// 

#pragma once

#include "CoreMinimal.h"
#include "SBaseComboBox.h"

class SBranchComboBox : public SBaseComboBox
{
public:
	SLATE_BEGIN_ARGS(SBranchComboBox) {}
	SLATE_ARGUMENT(TSharedPtr<FSmartDialogueEditor>, SmartDialogueEditor)
	SLATE_EVENT(FOnItemSelectedDelegate, OnItemSelected)
	SLATE_ARGUMENT(FString, DefaultText)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	virtual TArray<TSharedPtr<FString>> GetOptionsList() override;
};