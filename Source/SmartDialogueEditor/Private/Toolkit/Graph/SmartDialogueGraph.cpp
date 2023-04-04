// 


#include "SmartDialogueGraph.h"
#include "BranchNode.h"

#include "SmartDialogueGraphSchema.h"

USmartDialogueGraph::USmartDialogueGraph()
{
	Schema = USmartDialogueGraphSchema::StaticClass();
}

void USmartDialogueGraph::SetEditor(FSmartDialogueEditor* InEditor)
{
	Editor = InEditor;
}

UBranchNode* USmartDialogueGraph::CreateBranchNode() const
{	
	return NewObject<UBranchNode>()->Initialize(FName("branch_0"), Editor);
}
