// 


#include "FSmartDialogueEditorCommands.h"
#include "Framework/Commands/UICommandList.h"


#define LOCTEXT_NAMESPACE "SmartDialogueEditor"


void FSmartDialogueEditorCommands::RegisterCommands()
{
	UI_COMMAND(AddNewBranch, "Add New Branch", "Add a new dialogue branch", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(PlayDialogue, "Play", "Play the dialogue", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ShowConfig, "Config", "Show configuration cnaracters and variables", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE