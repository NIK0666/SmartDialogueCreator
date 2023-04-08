// SDialogueElementRow.h
#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "DialoguePlayerData.h"

DECLARE_DELEGATE_RetVal_OneParam(FSlateColor, FGetSpeakerNameColorDelegate, const FString&);
DECLARE_DELEGATE_RetVal_OneParam(EVisibility, FShouldHideSpeakerNameDelegate, int32 InIndex);

class SDialogueElementRow : public SCompoundWidget
{
public:
SLATE_BEGIN_ARGS(SDialogueElementRow) {}
	SLATE_ARGUMENT(FDialogueElementData, DialogueElementData)
SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
};