// SParameterListRow.h

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SParameterListRow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SParameterListRow) {}
	SLATE_ATTRIBUTE(FString, Parameter)
	SLATE_ATTRIBUTE(FString, Desc)
	SLATE_EVENT(FSimpleDelegate, OnDeleteButtonClicked)
SLATE_END_ARGS()

void Construct(const FArguments& InArgs);

private:
	TAttribute<FString> Parameter;
	TAttribute<FString> Desc;
	FSimpleDelegate OnDeleteButtonClicked;
};