// 


#include "SmartDialogueGraph.h"

#include "SmartDialogueGraphSchema.h"

USmartDialogueGraph::USmartDialogueGraph()
{
	Schema = USmartDialogueGraphSchema::StaticClass();
}

// UBranchNode* USmartDialogueGraph::CreateBranchNode(bool bisUserAction, bool bIsSelectNewNode)
// {
// 	UBranchNode* NewBranchNode = NewObject<UBranchNode>(this);
// 	AddNode(NewBranchNode, bisUserAction, bIsSelectNewNode);
// 	return NewBranchNode;
// }

void USmartDialogueGraph::SetEditor(FSmartDialogueEditor* InEditor)
{
	Editor = InEditor;
}
