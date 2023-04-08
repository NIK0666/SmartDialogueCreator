// 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

DECLARE_DELEGATE_OneParam(FOnSelectedAnswer, int32)

class SAnswerButtonRow : public SCompoundWidget
{
public:
SLATE_BEGIN_ARGS(SAnswerButtonRow){}
	SLATE_ARGUMENT(FText, AnswerText)
	SLATE_ARGUMENT(int32, AnswerIndex)
	SLATE_EVENT(FOnSelectedAnswer, OnSelectedAnswer)
SLATE_END_ARGS()

void Construct(const FArguments& InArgs);
};