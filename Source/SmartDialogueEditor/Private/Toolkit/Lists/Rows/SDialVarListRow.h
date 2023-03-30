// 

#pragma once

#include "CoreMinimal.h"

class SDialConfigWidget;

class SDialVarListRow : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_OneParam(FOnVarDeleted, TSharedPtr<FString>)

	
	SLATE_BEGIN_ARGS(SDialVarListRow) {}
	SLATE_ARGUMENT(FString, VarKey)
	SLATE_ARGUMENT(FString, VarValue)
	SLATE_ARGUMENT(FString, VarDesc)
	SLATE_EVENT(FSimpleDelegate, OnDeleteButtonClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FString GetVarKey();

private:
	TAttribute<FString> VarKey;
	TAttribute<FString> VarValue;
	TAttribute<FString> VarDesc;
	FSimpleDelegate OnDeleteButtonClicked;

};
