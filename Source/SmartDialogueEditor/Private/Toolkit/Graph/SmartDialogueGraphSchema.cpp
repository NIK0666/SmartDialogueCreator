// SmartDialogueGraphSchema.cpp


#include "SmartDialogueGraphSchema.h"

#include "BranchNode.h"
#include "EdGraph/EdGraph.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "USmartDialogueGraphSchema"

void USmartDialogueGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	// Add "Add Branch Node" action
	const FText Name = LOCTEXT("AddBranchNodeAction", "Add Branch Node");
	const FText ToolTip = LOCTEXT("AddBranchNodeTooltip", "Create a new Branch Node");
	const FText Category = LOCTEXT("DialogueNodesCategory", "Dialogue Nodes");

	TSharedPtr<FEdGraphSchemaAction_NewNode> AddBranchNodeAction = TSharedPtr<FEdGraphSchemaAction_NewNode>(new FEdGraphSchemaAction_NewNode(Category, Name, ToolTip, 0));
	AddBranchNodeAction->NodeTemplate = NewObject<UBranchNode>();

	ContextMenuBuilder.AddAction(AddBranchNodeAction);
}

TSharedPtr<FEdGraphSchemaAction> USmartDialogueGraphSchema::GetCreateCommentAction() const
{
	return TSharedPtr<FEdGraphSchemaAction>(new FEdGraphSchemaAction_NewNode);
}

void USmartDialogueGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	if (Context->Node)
	{
		FToolMenuSection& Section = Menu->AddSection("SmartDialogueGraphSchemaNodeActions", LOCTEXT("NodeActionsMenuHeader", "Node Actions"));
		{
			
		}
	}
}

#undef LOCTEXT_NAMESPACE