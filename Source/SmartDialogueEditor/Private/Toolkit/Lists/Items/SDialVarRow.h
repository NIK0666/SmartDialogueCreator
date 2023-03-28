// 

#pragma once

#include "CoreMinimal.h"

class SDialConfigWidget;

class SDialVarRow : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_OneParam(FOnVarDeleted, TSharedPtr<FString>)

	
	SLATE_BEGIN_ARGS(SDialVarRow) {}
	SLATE_ARGUMENT(TSharedPtr<FString>, VarId)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	TSharedPtr<FString> GetVarId();

private:
	FReply OnDeleteButtonClicked();

	TSharedPtr<FString> VarId;
};

