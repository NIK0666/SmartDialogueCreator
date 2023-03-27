// 

#pragma once

#include "CoreMinimal.h"
// #include "SPhraseListItemWidget.generated.h"

class SPhraseListItemWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPhraseListItemWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
};
