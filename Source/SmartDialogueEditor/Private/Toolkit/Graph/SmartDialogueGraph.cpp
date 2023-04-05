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
	TQueue<UBranchNode*> NodesToProcess;
	TMap<FName, int32> NodeLevels;
	TMap<int32, TArray<UBranchNode*>> NodesByLevel;

	// Шаг 1: Найдите все начальные ноды
	for (UEdGraphNode* Node : Nodes)
	{
		UBranchNode* BranchNode = Cast<UBranchNode>(Node);
		if (BranchNode && BranchNode->FindPin(UEdGraphSchema_K2::PN_Execute)->LinkedTo.Num() == 0)
		{
			NodesToProcess.Enqueue(BranchNode);
			NodeLevels.Add(BranchNode->GetBranchName(), 0);
			NodesByLevel.FindOrAdd(0).Add(BranchNode);
		}
	}

	// Шаги 2-3: Обработка нод
	while (!NodesToProcess.IsEmpty())
	{
		UBranchNode* CurrentNode;
		NodesToProcess.Dequeue(CurrentNode);
		int32 CurrentLevel = NodeLevels[CurrentNode->GetBranchName()];
		
		UEdGraphPin* ShowPin = CurrentNode->FindPin(UEdGraphSchema_K2::PN_Then);
		
		for (UEdGraphPin* Pin : ShowPin->LinkedTo)
		{
			UBranchNode* LinkedBranchNode = Cast<UBranchNode>(Pin->GetOwningNode());
			if (LinkedBranchNode && !NodeLevels.Contains(LinkedBranchNode->GetBranchName()))
			{
				NodesToProcess.Enqueue(LinkedBranchNode);
				NodeLevels.Add(LinkedBranchNode->GetBranchName(), CurrentLevel + 1);
				NodesByLevel.FindOrAdd(CurrentLevel + 1).Add(LinkedBranchNode);
			}
		}
	}

	// Шаг 4: Сортировка нод на каждом уровне по вертикальной координате
	for (auto& LevelNodesPair : NodesByLevel)
	{
		LevelNodesPair.Value.Sort([](const UBranchNode& A, const UBranchNode& B) {
			return A.NodePosY < B.NodePosY;
		});
	}

	// Шаг 5: Присвоение новых позиций нодам
	float HorizontalSpacing = 384.0f;
	float VerticalSpacing = 128.0f;
	for (auto& LevelNodesPair : NodesByLevel)
	{
		int32 Level = LevelNodesPair.Key;
		float LevelX = Level * HorizontalSpacing;
		for (int32 i = 0; i < LevelNodesPair.Value.Num(); ++i)
		{
			UBranchNode* Node = LevelNodesPair.Value[i];
			float NewY = i * VerticalSpacing;
			Node->NodePosX = LevelX;
			Node->NodePosY = NewY;
		}
	}
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

FString USmartDialogueGraph::GetNodesInformation()
{
	FString NodesInfo;

	// Получение нод и их соединений
	TArray<UEdGraphNode*> AllNodes = Nodes;
	for (UEdGraphNode* Node : AllNodes)
	{
		if (UBranchNode* BranchNode = Cast<UBranchNode>(Node))
		{
			FVector2D NodePosition = FVector2D(BranchNode->NodePosX, BranchNode->NodePosY);
			FVector2D NodeSize = BranchNode->GetNodeSize();
			FName NodeName = BranchNode->GetBranchName();

			FString InNodes, OutNodes;

			for (UEdGraphPin* Pin : BranchNode->Pins)
			{
				if (Pin->Direction == EGPD_Input)
				{
					for (UEdGraphPin* LinkedPin : Pin->LinkedTo)
					{
						UBranchNode* LinkedBranchNode = Cast<UBranchNode>(LinkedPin->GetOwningNode());
						if (LinkedBranchNode)
						{
							InNodes += FString::Printf(TEXT("\"%s\", "), *LinkedBranchNode->GetBranchName().ToString());
						}
					}
				}
				else if (Pin->Direction == EGPD_Output)
				{
					for (UEdGraphPin* LinkedPin : Pin->LinkedTo)
					{
						UBranchNode* LinkedBranchNode = Cast<UBranchNode>(LinkedPin->GetOwningNode());
						if (LinkedBranchNode)
						{
							OutNodes += FString::Printf(TEXT("\"%s\", "), *LinkedBranchNode->GetBranchName().ToString());
						}
					}
				}
			}

			// Удаление последней запятой и пробела
			if (!InNodes.IsEmpty()) InNodes.RemoveAt(InNodes.Len() - 2, 2);
			if (!OutNodes.IsEmpty()) OutNodes.RemoveAt(OutNodes.Len() - 2, 2);

			NodesInfo += FString::Printf(
				TEXT("{\nName: \"%s\"\nPos: (%.1f, %.1f)\nSize: (%.1f, %.1f)\nIn: [%s]\nOut: [%s]\n},\n"),
				*NodeName.ToString(), NodePosition.X, NodePosition.Y, NodeSize.X, NodeSize.Y, *InNodes, *OutNodes);
		}
	}

	return NodesInfo;
}
