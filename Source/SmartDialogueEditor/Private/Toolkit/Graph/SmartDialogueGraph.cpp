// 


#include "SmartDialogueGraph.h"
#include "BranchNode.h"

#include "SmartDialogueGraphSchema.h"
#include "Settings/EditorStyleSettings.h"
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
    UBranchNode* ResultNode = NewObject<UBranchNode>(const_cast<USmartDialogueGraph*>(this));
    ResultNode->Initialize(BranchName, Editor);
	ResultNode->AllocateDefaultPins();
    ResultNode->NodePosX = LastNodePos.X;
    ResultNode->NodePosY = LastNodePos.Y;
	
	ResultNode->CreateNewGuid();
	ResultNode->PostPlacedNewNode();
	ResultNode->AutowireNewNode(nullptr);
	ResultNode->SnapToGrid(GetDefault<UEditorStyleSettings>()->GridSnapSize);
    return ResultNode;
}

void USmartDialogueGraph::AddBranchNode(FSmartDialogueBranch& NewBranch)
{
	UBranchNode* NewNode = CreateBranchNode(NewBranch.Name);
	this->AddNode(NewNode, true, false);
	this->NotifyGraphChanged();

	LastNodePos.Y += 150.f;
}
