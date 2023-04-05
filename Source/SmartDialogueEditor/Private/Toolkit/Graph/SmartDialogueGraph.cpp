// 


#include "SmartDialogueGraph.h"
#include "BranchNode.h"

#include "SmartDialogueGraphSchema.h"
#include "Toolkit/FSmartDialogueEditor.h"

USmartDialogueGraph::USmartDialogueGraph()
{
	Schema = USmartDialogueGraphSchema::StaticClass();
}

void USmartDialogueGraph::SetEditor(FSmartDialogueEditor* InEditor)
{
	Editor = InEditor;

	if (Editor)
	{
		Editor->OnBranchItemAdded.AddUObject(this, &USmartDialogueGraph::AddBranchNode);
	}
}

UBranchNode* USmartDialogueGraph::CreateBranchNode(const FName& BranchName) const
{
    UBranchNode* NewNode = NewObject<UBranchNode>(const_cast<USmartDialogueGraph*>(this));
    NewNode->Initialize(BranchName, Editor);
    NewNode->NodePosX = LastMousePos.X;
    NewNode->NodePosY = LastMousePos.Y;
    return NewNode;
}

void USmartDialogueGraph::AddBranchNode(FSmartDialogueBranch& NewBranch)
{
	UBranchNode* NewNode = CreateBranchNode(NewBranch.Name);
	this->AddNode(NewNode, true, false);
	this->NotifyGraphChanged();
}
