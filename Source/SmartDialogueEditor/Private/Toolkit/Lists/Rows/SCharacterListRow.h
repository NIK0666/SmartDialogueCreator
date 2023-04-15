// SCharacterListRow.h

#pragma once

#include "CoreMinimal.h"
#include "SmartDialogueData.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

DECLARE_DELEGATE_OneParam(FCharacterDataChanged, const FCharacterData& UpdatedCharacter)

class SCharacterListRow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCharacterListRow) {}
	SLATE_ATTRIBUTE(FString, Id)
	SLATE_ATTRIBUTE(FText, Name)
	SLATE_EVENT(FSimpleDelegate, OnDeleteButtonClicked)
	SLATE_EVENT(FCharacterDataChanged, OnChanged)
SLATE_END_ARGS()

void Construct(const FArguments& InArgs);
	
	FSimpleDelegate OnDeleteButtonClicked;
	FCharacterDataChanged OnChanged;
	
private:
	TAttribute<FString> Id;
	TAttribute<FText> Name;
	
	TSharedPtr<SEditableTextBox> IdEditableTextBox;
	TSharedPtr<SEditableTextBox> NameEditableTextBox;
};