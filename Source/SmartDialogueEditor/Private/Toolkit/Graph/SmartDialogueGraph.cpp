// 


#include "SmartDialogueGraph.h"
#include "BranchNode.h"
#include "SGraphNode.h"
#include "SmartDialogue.h"

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

		LoadNodesFromAsset();
	}
}

void USmartDialogueGraph::LoadNodesFromAsset()
{
	if (!Editor)
	{
		return;
	}

	USmartDialogue* Dialogue = Editor->GetDialogue();
	if (Dialogue)
	{
		bIsInitializeGraph = true;
		for (const TPair<FName, FSmartDialogueBranch>& Pair : Dialogue->GetBranches())
		{
			FSmartDialogueBranch Branch = Pair.Value;
			AddBranchNode(Branch);
		}
		
		bIsInitializeGraph = false;
		
		CreateConnections();
		SortNodes();
	}
}

void USmartDialogueGraph::CreateConnections()
{
	if (!Editor)
	{
		return;
	}

	USmartDialogue* Dialogue = Editor->GetDialogue();
	if (Dialogue)
	{
		for (const TPair<FName, FSmartDialogueBranch>& Pair : Dialogue->GetBranches())
		{
			FSmartDialogueBranch Branch = Pair.Value;
			UBranchNode* SourceNode = GetBranchNodeByName(Pair.Key);

			if (SourceNode)
			{
				UEdGraphPin* ShowPin = SourceNode->FindPin(UEdGraphSchema_K2::PN_Then);
				UEdGraphPin* HidePin = SourceNode->FindPin(UEdGraphSchema_K2::PN_Else);

				for (const FString& TargetBranchName : Branch.Show)
				{
					UBranchNode* TargetNode = GetBranchNodeByName(FName(*TargetBranchName));
					if (TargetNode)
					{
						UEdGraphPin* InputPin = TargetNode->FindPin(UEdGraphSchema_K2::PN_Execute);
						ShowPin->MakeLinkTo(InputPin);
					}
				}

				for (const FString& TargetBranchName : Branch.Hide)
				{
					UBranchNode* TargetNode = GetBranchNodeByName(FName(*TargetBranchName));
					if (TargetNode)
					{
						UEdGraphPin* InputPin = TargetNode->FindPin(UEdGraphSchema_K2::PN_Execute);
						HidePin->MakeLinkTo(InputPin);
					}
				}
			}
		}
	}
}

void USmartDialogueGraph::SortNodes()
{
	const float XOffset = 200.0f;
	const float YOffset = 150.0f;

	TArray<UBranchNode*> SortedNodes;
	TArray<UBranchNode*> UnsortedNodes;

	// Находим все ноды без входных связей и добавляем их в список SortedNodes
	for (UEdGraphNode* Node : Nodes)
	{
		UBranchNode* BranchNode = Cast<UBranchNode>(Node);
		if (BranchNode && BranchNode->FindPin(UEdGraphSchema_K2::PN_Execute)->LinkedTo.Num() == 0)
		{
			SortedNodes.Add(BranchNode);
		}
		else
		{
			UnsortedNodes.Add(BranchNode);
		}
	}

	// Располагаем ноды без входных связей по Y
	float CurrentY = 0.0f;
	for (UBranchNode* Node : SortedNodes)
	{
		Node->NodePosX = 0;
		Node->NodePosY = CurrentY;
		CurrentY += Node->GetNodeSize().Y + YOffset;
	}

	// Располагаем ноды с Show-связями
	for (int32 Index = 0; Index < SortedNodes.Num(); ++Index)
	{
		UBranchNode* ParentNode = SortedNodes[Index];
		UEdGraphPin* ShowPin = ParentNode->FindPin(UEdGraphSchema_K2::PN_Then);

		int32 ChildNodesCount = ShowPin->LinkedTo.Num();
		float ChildNodesTotalHeight = 0.0f;
		for (int32 ChildIndex = 0; ChildIndex < ChildNodesCount; ++ChildIndex)
		{
			UEdGraphPin* LinkedPin = ShowPin->LinkedTo[ChildIndex];
			UBranchNode* ChildNode = Cast<UBranchNode>(LinkedPin->GetOwningNode());
			if (ChildNode)
			{
				ChildNodesTotalHeight += ChildNode->GetNodeSize().Y + YOffset;
			}
		}

		float ChildNodesStartY = ParentNode->NodePosY - ChildNodesTotalHeight / 2.0f + ParentNode->GetNodeSize().Y / 2.0f;

		for (int32 ChildIndex = 0; ChildIndex < ChildNodesCount; ++ChildIndex)
		{
			UEdGraphPin* LinkedPin = ShowPin->LinkedTo[ChildIndex];
			UBranchNode* ChildNode = Cast<UBranchNode>(LinkedPin->GetOwningNode());

			if (ChildNode)
			{
				ChildNode->NodePosX = ParentNode->NodePosX + ParentNode->GetNodeSize().X + XOffset;
				ChildNode->NodePosY = ChildNodesStartY;

				SortedNodes.Add(ChildNode);
				UnsortedNodes.Remove(ChildNode);

				ChildNodesStartY += ChildNode->GetNodeSize().Y + YOffset;
			}
		}
	}

	// Если еще остались нерасположенные ноды (возможно, имеют только Hide-связи), добавляем их к списку SortedNodes
	for (UBranchNode* Node : UnsortedNodes)
	{
		SortedNodes.Add(Node);
	}

	// Заменяем текущий список нод отсортированным списком
	TArray<UEdGraphNode*> NewNodes;
	NewNodes.Reserve(Nodes.Num());
	for (UBranchNode* Node : SortedNodes)
	{
		NewNodes.Add(Node);
		Nodes.Remove(Node);
	}

	// Добавляем оставшиеся ноды, которые не являются UBranchNode
	NewNodes.Append(Nodes);
	Nodes = MoveTemp(NewNodes);

}

UBranchNode* USmartDialogueGraph::GetBranchNodeByName(FName BranchName) const
{
	for (UEdGraphNode* Node : Nodes)
	{
		UBranchNode* BranchNode = Cast<UBranchNode>(Node);
		if (BranchNode && BranchNode->GetBranchName() == BranchName)
		{
			return BranchNode;
		}
	}

	return nullptr;
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
	this->AddNode(NewNode, !bIsInitializeGraph, false);
	this->NotifyGraphChanged();

	LastNodePos.Y += 150.f;
}