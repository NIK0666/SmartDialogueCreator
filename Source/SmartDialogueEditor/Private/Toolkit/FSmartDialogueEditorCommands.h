// FSmartDialogueEditorCommands.cpp

#pragma once

#include "CoreMinimal.h"
#include "EditorStyleSet.h"

class FSmartDialogueEditorCommands : public TCommands<FSmartDialogueEditorCommands>
{
public:
	FSmartDialogueEditorCommands()
		: TCommands<FSmartDialogueEditorCommands>(TEXT("SmartDialogueEditor"), NSLOCTEXT("Contexts", "SmartDialogueEditor", "Smart Dialogue Editor"), NAME_None, FEditorStyle::GetStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> AddNewBranch;
	TSharedPtr<FUICommandInfo> PlayDialogue;
	TSharedPtr<FUICommandInfo> ShowConfig;
};
