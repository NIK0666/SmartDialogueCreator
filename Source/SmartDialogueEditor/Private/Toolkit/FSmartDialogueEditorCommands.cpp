// 


#include "FSmartDialogueEditorCommands.h"
#include "Framework/Commands/UICommandList.h"


#define LOCTEXT_NAMESPACE "SmartDialogueEditor"


void FSmartDialogueEditorCommands::RegisterCommands()
{
	UI_COMMAND(AddNewBranch, "Add New Branch", "Add a new dialogue branch", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ShowBranches, "Show Branches", "Show all branches", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(PlayDialogue, "Play", "Play the dialogue", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ShowConfig, "Config", "Show configuration cnaracters and variables", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ImportJSON, "Import from JSON", "Import data into the current dialogue from a JSON file", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(CopyNodesInfo, "Copy Nodes Info", "Copies the information of the nodes", EUserInterfaceActionType::Button, FInputChord(EKeys::C, EModifierKey::Control));
	UI_COMMAND(SortNodes, "Sort Nodes", "Sort nodes in the graph", EUserInterfaceActionType::Button, FInputChord());

}

#undef LOCTEXT_NAMESPACE