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
        
		// Добавляем действие "Удалить ноду"
		Section.AddMenuEntry(
			"DeleteNode",
			LOCTEXT("DeleteNodeAction", "Delete"),
			LOCTEXT("DeleteNodeTooltip", "Delete selected node."),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateLambda([Context]()
				{
					UBranchNode* OriginalNode = Cast<UBranchNode>(Context->Node);
					OriginalNode->Modify();
					OriginalNode->DestroyNode();
				}),
				FCanExecuteAction()
			)
		);

		// Добавляем действие "Дублировать ноду"
		Section.AddMenuEntry(
			"DuplicateNode",
			LOCTEXT("DuplicateNodeAction", "Duplicate"),
			LOCTEXT("DuplicateNodeTooltip", "Duplicate selected node."),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateLambda([Context]()
				{
					UEdGraph* Graph = const_cast<UEdGraph*>(Context->Graph.Get());
					UBranchNode* OriginalNode = Cast<UBranchNode>(Context->Node);
					UBranchNode* DuplicatedNode = DuplicateObject<UBranchNode>(OriginalNode, Graph);
					DuplicatedNode->SetFlags(RF_Transactional);
					Graph->AddNode(DuplicatedNode, true, false);

					DuplicatedNode->CreateNewGuid();
					DuplicatedNode->PostPasteNode();
					DuplicatedNode->AllocateDefaultPins();
					DuplicatedNode->NodePosX += 50;
					DuplicatedNode->NodePosY += 50;
				}),
				FCanExecuteAction::CreateLambda([Context]() { return Context->Node->CanDuplicateNode(); })
			)
		);
	}
}

#undef LOCTEXT_NAMESPACE