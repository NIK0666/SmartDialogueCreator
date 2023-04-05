// 


#include "EdGraphSchemaAction_AddBranchNode.h"

#include "SmartDialogueGraph.h"
#include "Toolkit/FSmartDialogueEditor.h"

UEdGraphNode* FEdGraphSchemaAction_AddBranchNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
                                                                const FVector2D Location, bool bSelectNewNode)
{
	auto* DialogueGraph = Cast<USmartDialogueGraph>(ParentGraph);
	if (DialogueGraph && DialogueGraph->GetEditor())
	{
		DialogueGraph->LastMousePos = Location;
		DialogueGraph->GetEditor()->AddNewBranch();
	}
	return nullptr;
}


