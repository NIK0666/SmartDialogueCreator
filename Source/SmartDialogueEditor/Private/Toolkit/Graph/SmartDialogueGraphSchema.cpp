// SmartDialogueGraphSchema.cpp


#include "SmartDialogueGraphSchema.h"

#include "BranchNode.h"
#include "EdGraphSchemaAction_AddBranchNode.h"
#include "SmartDialogueGraph.h"
#include "EdGraph/EdGraph.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "ToolMenus.h"
#include "Toolkit/FSmartDialogueEditor.h"
#include "Toolkit/FSmartDialogueEditorCommands.h"

#define LOCTEXT_NAMESPACE "USmartDialogueGraphSchema"

void USmartDialogueGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	auto* DialogueGraph = Cast<USmartDialogueGraph>(ContextMenuBuilder.CurrentGraph);
	check(DialogueGraph);

	const TSharedPtr<FEdGraphSchemaAction> AddBranchNodeAction = MakeShareable(new FEdGraphSchemaAction_AddBranchNode());
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
					DuplicatedNode->NodePosX += 100;
					DuplicatedNode->NodePosY += 10;
				}),
				FCanExecuteAction::CreateLambda([Context]() { return Context->Node->CanDuplicateNode(); })
			)
		);
	}
}

const FPinConnectionResponse USmartDialogueGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	// Ensure both pins are valid
	if (!A || !B)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Invalid pin(s)"));
	}

	// Disallow multiple connections between the same pair of nodes
	UEdGraphNode* NodeA = A->GetOwningNode();
	UEdGraphNode* NodeB = B->GetOwningNode();
	for (UEdGraphPin* NodeAPin : NodeA->Pins)
	{
		if (NodeAPin->Direction == EGPD_Output)
		{
			for (UEdGraphPin* NodeBPin : NodeB->Pins)
			{
				if (NodeBPin->Direction == EGPD_Input && NodeAPin->LinkedTo.Contains(NodeBPin))
				{
					return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Nodes are already connected"));
				}
			}
		}
		else
		{
			for (UEdGraphPin* NodeBPin : NodeB->Pins)
			{
				if (NodeBPin->Direction == EGPD_Output && NodeAPin->LinkedTo.Contains(NodeBPin))
				{
					return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Nodes are already connected"));
				}
			}
		}
	}

	// Disallow connections to self
	if (NodeA == NodeB)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot connect node to itself"));
	}

	// Disallow multiple connections to the same input pin
	if (A->Direction == EGPD_Input && B->Direction == EGPD_Input)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Both pins are input pins"));
	}

	// Disallow connections between two output pins
	if (A->Direction == EGPD_Output && B->Direction == EGPD_Output)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Both pins are output pins"));
	}

	// Allow the connection if the pins are execution pins
	if (A->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec && B->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
	}

	// For other pin types, defer to the parent class (UEdGraphSchema_K2)
	return Super::CanCreateConnection(A, B);
}

bool USmartDialogueGraphSchema::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
	return UEdGraphSchema::TryCreateConnection(A, B);
}

void USmartDialogueGraphSchema::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const
{
	UEdGraphSchema::BreakPinLinks(TargetPin, bSendsNodeNotifcation);
}


#undef LOCTEXT_NAMESPACE
