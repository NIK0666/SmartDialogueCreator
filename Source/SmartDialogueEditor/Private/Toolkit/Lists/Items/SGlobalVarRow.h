// 

#pragma once

#include "CoreMinimal.h"

class SDialConfigWidget;

class SGlobalVarRow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGlobalVarRow) {}
	SLATE_ARGUMENT(TSharedPtr<FString>, VarId)
	SLATE_ARGUMENT(TWeakPtr<SDialConfigWidget>, ParentWidget)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	TSharedPtr<FString> GetVarId();

private:
	FReply OnDeleteButtonClicked();

	TSharedPtr<FString> VarId;
	TWeakPtr<SDialConfigWidget> ParentWidget;
};

