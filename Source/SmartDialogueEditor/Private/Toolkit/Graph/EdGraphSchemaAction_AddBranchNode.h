// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#define LOCTEXT_NAMESPACE "USmartDialogueGraphSchema"

class FEdGraphSchemaAction_AddBranchNode : public FEdGraphSchemaAction
{
public:
	FEdGraphSchemaAction_AddBranchNode()
			: FEdGraphSchemaAction(
				LOCTEXT("DialogueNodesCategory", "Dialogue Nodes"),
				LOCTEXT("AddBranchNodeAction", "Add Branch Node"),
				LOCTEXT("AddBranchNodeTooltip", "Create a new Branch Node"),
				0)
	{
	}
	// В этом методе мы вызываем FSmartDialogueEditor::AddNewBranch
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode) override;
};

#undef LOCTEXT_NAMESPACE
