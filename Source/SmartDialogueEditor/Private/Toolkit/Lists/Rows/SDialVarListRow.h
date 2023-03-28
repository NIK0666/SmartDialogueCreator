// 

#pragma once

#include "CoreMinimal.h"

class SDialConfigWidget;

class SDialVarListRow : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_OneParam(FOnVarDeleted, TSharedPtr<FString>)

	
	SLATE_BEGIN_ARGS(SDialVarListRow) {}
	SLATE_ARGUMENT(TSharedPtr<FString>, VarId)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	TSharedPtr<FString> GetVarId();

private:
	FReply OnDeleteButtonClicked();

	TSharedPtr<FString> VarId;
};

