// SCharacterListRow.h

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SCharacterListRow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCharacterListRow) {}
	SLATE_ATTRIBUTE(FString, Id)
	SLATE_ATTRIBUTE(FText, Name)
	SLATE_EVENT(FSimpleDelegate, OnDeleteButtonClicked)
SLATE_END_ARGS()

void Construct(const FArguments& InArgs);

private:
	TAttribute<FString> Id;
	TAttribute<FText> Name;
	FSimpleDelegate OnDeleteButtonClicked;
};