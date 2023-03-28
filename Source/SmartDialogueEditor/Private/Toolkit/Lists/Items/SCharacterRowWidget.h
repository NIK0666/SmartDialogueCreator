// SCharacterRowWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SCharacterRowWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCharacterRowWidget) {}
	SLATE_ATTRIBUTE(FString, Id)
	SLATE_ATTRIBUTE(FString, Name)
	SLATE_EVENT(FSimpleDelegate, OnDeleteButtonClicked)
SLATE_END_ARGS()

void Construct(const FArguments& InArgs);

private:
	TAttribute<FString> Id;
	TAttribute<FString> Name;
	FSimpleDelegate OnDeleteButtonClicked;
};