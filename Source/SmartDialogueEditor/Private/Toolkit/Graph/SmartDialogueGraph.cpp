// 


#include "SmartDialogueGraph.h"
#include "BranchNode.h"
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
		
		Editor->GetDialogue()->OnShowBranchAdded.AddUObject(this, &USmartDialogueGraph::OnShowBranchAdded);
		Editor->GetDialogue()->OnHideBranchAdded.AddUObject(this, &USmartDialogueGraph::OnHideBranchAdded);
		Editor->GetDialogue()->OnHideBranchRemoved.AddUObject(this, &USmartDialogueGraph::OnHideBranchRemoved);
		Editor->GetDialogue()->OnShowBranchRemoved.AddUObject(this, &USmartDialogueGraph::OnShowBranchRemoved);
		Editor->GetDialogue()->OnShowBranchUpdated.AddUObject(this, &USmartDialogueGraph::OnShowBranchUpdated);
		Editor->GetDialogue()->OnHideBranchUpdated.AddUObject(this, &USmartDialogueGraph::OnHideBranchUpdated);
		Editor->GetDialogue()->OnBranchRemoved.AddUObject(this, &USmartDialogueGraph::OnBranchRemoved);
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

void USmartDialogueGraph::PositionNode(UEdGraphNode* Node, int32 X, int32 Y, TSet<UEdGraphNode*>& ProcessedNodes, int32& MaxY)
{
	if (ProcessedNodes.Contains(Node))
	{
		return;
	}
	ProcessedNodes.Add(Node);

	Node->NodePosX = X * 384;
	Node->NodePosY = Y * 128;

	MaxY = FMath::Max(MaxY, Y);

	int32 ChildY = Y;
	for (UEdGraphPin* OutputPin : Node->GetAllPins())
	{
		if (OutputPin->Direction == EGPD_Output && OutputPin->LinkedTo.Num() > 0)
		{
			for (UEdGraphPin* LinkedPin : OutputPin->LinkedTo)
			{
				if (!ProcessedNodes.Contains(LinkedPin->GetOwningNode()))
				{
					PositionNode(LinkedPin->GetOwningNode(), X + 1, ChildY, ProcessedNodes, MaxY);
					ChildY++;
				}
			}
		}
	}
}

void USmartDialogueGraph::SortNodes()
{
	TSet<UEdGraphNode*> ProcessedNodes;
	int32 CurrentY = 0;
	int32 MaxY = -1;

	while (ProcessedNodes.Num() != Nodes.Num())
	{
		UEdGraphNode* StartNode = nullptr;
		for (UEdGraphNode* Node : Nodes)
		{
			if (!ProcessedNodes.Contains(Node) && Node->GetAllPins().FindByPredicate([](UEdGraphPin* Pin) {
				return Pin->Direction == EGPD_Input && Pin->LinkedTo.Num() > 0;
			}) == nullptr)
			{
				StartNode = Node;
				break;
			}
		}

		if (StartNode)
		{
			CurrentY = MaxY + 1;
			PositionNode(StartNode, 0, CurrentY, ProcessedNodes, MaxY);
		}
		else
		{
			break;
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


UBranchNode* USmartDialogueGraph::CreateBranchNode(const FName& BranchName)
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

	// Соединяем пины, если FromPin был предоставлен
	if (LastFromPin)
	{
		if (LastFromPin->Direction == EGPD_Output)
		{
			UEdGraphPin* ToPin = ResultNode->FindPin(UEdGraphSchema_K2::PN_Execute);
			if (ToPin)
			{
				LastFromPin->MakeLinkTo(ToPin);
				ResultNode->PinConnectionListChanged(ToPin);
				if (auto OwnBranchNode = Cast<UBranchNode>(LastFromPin->GetOwningNode()))
				{
					if (LastFromPin->PinName == UEdGraphSchema_K2::PN_Then)
					{
						Editor->GetDialogue()->AddShowBranchElement(OwnBranchNode->GetBranchName(), BranchName.ToString());
					}
					else if (LastFromPin->PinName == UEdGraphSchema_K2::PN_Else)
					{
						Editor->GetDialogue()->AddHideBranchElement(OwnBranchNode->GetBranchName(), BranchName.ToString());
					}
				}
				
				//Проверить пин, еshow или hide и добавить новый элемент show или hide
			}			
		}
		LastFromPin = nullptr;
	}
	
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
				TEXT("{Name: \"%s\",Pos: (%d, %.d),In: [%s],Out: [%s]}\n"),
				*NodeName.ToString(), int32(NodePosition.X), int32(NodePosition.Y), *InNodes, *OutNodes);
		}
	}

	return NodesInfo;
}

void USmartDialogueGraph::OnUserConnectPins(UEdGraphPin* A, UEdGraphPin* B)
{
	if (!A || !B) {	return;	}
	
	UEdGraphPin* InputPin =  (A->Direction == EGPD_Input) ? A : B;
	UEdGraphPin* OutputPin =  (A->Direction == EGPD_Output) ? A : B;
	
	if (!InputPin || !OutputPin) { return; }
	
	UBranchNode* InputNode = Cast<UBranchNode>(InputPin->GetOwningNode());
	UBranchNode* OutputNode = Cast<UBranchNode>(OutputPin->GetOwningNode());
	
	if (!InputNode || !OutputNode) { return; }

	// TODO Решение не идеальное, но почему то PinName = none
	const int32 OutputIndex = OutputNode->Pins.Find(OutputPin);	
	if (OutputIndex == 1)
	{
		Editor->GetDialogue()->AddShowBranchElement(OutputNode->GetBranchName(), InputNode->GetBranchName().ToString());
	}
	else if (OutputIndex == 2)
	{
		Editor->GetDialogue()->AddHideBranchElement(OutputNode->GetBranchName(), InputNode->GetBranchName().ToString());
	}
}

void USmartDialogueGraph::OnUserDisconnectPins(UEdGraphPin* A, UEdGraphPin* B)
{
	if (!A || !B) {	return;	}
	
	UEdGraphPin* InputPin =  (A->Direction == EGPD_Input) ? A : B;
	UEdGraphPin* OutputPin =  (A->Direction == EGPD_Output) ? A : B;
	
	if (!InputPin || !OutputPin) { return; }
	
	UBranchNode* InputNode = Cast<UBranchNode>(InputPin->GetOwningNode());
	UBranchNode* OutputNode = Cast<UBranchNode>(OutputPin->GetOwningNode());
	
	if (!InputNode || !OutputNode) { return; }

	// TODO Решение не идеальное, но почему то PinName = none
	const int32 OutputIndex = OutputNode->Pins.Find(OutputPin);	
	if (OutputIndex == 1)
	{		
		Editor->GetDialogue()->RemoveShowBranchByString(OutputNode->GetBranchName(), InputNode->GetBranchName().ToString());
	}
	else if (OutputIndex == 2)
	{
		Editor->GetDialogue()->RemoveHideBranchByString(OutputNode->GetBranchName(), InputNode->GetBranchName().ToString());
	}
}

void USmartDialogueGraph::HandleSelectedNodesChanged(const TSet<UObject*>& NewSelection)
{
	if (NewSelection.Num() != 1)
	{
		return;
	}

	UObject* SelectedObject = *NewSelection.CreateConstIterator();
	UBranchNode* BranchNode = Cast<UBranchNode>(SelectedObject);
    
	if (BranchNode)
	{
		Editor->SetSelectedBranchName(BranchNode->GetBranchName());
	}
}

// Удаляет связь от выхода UEdGraphSchema_K2::PN_Then ноды с именем BranchName до входа UEdGraphSchema_K2::PN_Execute узла с именем ToName
void USmartDialogueGraph::OnShowBranchRemoved(FName BranchName, int32 Index, FString Value)
{
	FName ToName = FName(Value);
	UBranchNode* SourceNode = GetBranchNodeByName(BranchName);
	UBranchNode* TargetNode = GetBranchNodeByName(ToName);

	if (SourceNode && TargetNode)
	{
		UEdGraphPin* ShowPin = SourceNode->FindPin(UEdGraphSchema_K2::PN_Then);
		UEdGraphPin* InputPin = TargetNode->FindPin(UEdGraphSchema_K2::PN_Execute);

		if (ShowPin->LinkedTo.Contains(InputPin))
		{
			ShowPin->BreakLinkTo(InputPin);
		}
	}
}

// Добавляет связь от выхода UEdGraphSchema_K2::PN_Then ноды с именем BranchName до входа UEdGraphSchema_K2::PN_Execute узла с именем ToName
void USmartDialogueGraph::OnShowBranchAdded(FName BranchName, FString String)
{
	FName ToName = FName(String);
	UBranchNode* SourceNode = GetBranchNodeByName(BranchName);
	UBranchNode* TargetNode = GetBranchNodeByName(ToName);

	if (SourceNode && TargetNode)
	{
		UEdGraphPin* ShowPin = SourceNode->FindPin(UEdGraphSchema_K2::PN_Then);
		UEdGraphPin* InputPin = TargetNode->FindPin(UEdGraphSchema_K2::PN_Execute);

		if (!ShowPin->LinkedTo.Contains(InputPin))
		{
			ShowPin->MakeLinkTo(InputPin);
		}
	}
}

// Удаляет связь от выхода UEdGraphSchema_K2::PN_Else ноды с именем BranchName до входа UEdGraphSchema_K2::PN_Execute узла с именем ToName
void USmartDialogueGraph::OnHideBranchRemoved(FName BranchName, int32 Index, FString Value)
{
	FName ToName = FName(Value);
	UBranchNode* SourceNode = GetBranchNodeByName(BranchName);
	UBranchNode* TargetNode = GetBranchNodeByName(ToName);

	if (SourceNode && TargetNode)
	{
		UEdGraphPin* HidePin = SourceNode->FindPin(UEdGraphSchema_K2::PN_Else);
		UEdGraphPin* InputPin = TargetNode->FindPin(UEdGraphSchema_K2::PN_Execute);

		if (HidePin->LinkedTo.Contains(InputPin))
		{
			HidePin->BreakLinkTo(InputPin);
		}
	}
}

// Добавляет связь от выхода UEdGraphSchema_K2::PN_Else ноды с именем BranchName до входа UEdGraphSchema_K2::PN_Execute узла с именем ToName
void USmartDialogueGraph::OnHideBranchAdded(FName BranchName, FString String)
{
	FName ToName = FName(String);
	UBranchNode* SourceNode = GetBranchNodeByName(BranchName);
	UBranchNode* TargetNode = GetBranchNodeByName(ToName);

	if (SourceNode && TargetNode)
	{
		UEdGraphPin* ShowPin = SourceNode->FindPin(UEdGraphSchema_K2::PN_Else);
		UEdGraphPin* InputPin = TargetNode->FindPin(UEdGraphSchema_K2::PN_Execute);

		if (!ShowPin->LinkedTo.Contains(InputPin))
		{
			ShowPin->MakeLinkTo(InputPin);
		}
	}
}

void USmartDialogueGraph::OnShowBranchUpdated(FName BranchName, int32 Index, FString OldValue, FString NewValue)
{
	OnShowBranchRemoved(BranchName, Index, OldValue);
	OnShowBranchAdded(BranchName, NewValue);
}

void USmartDialogueGraph::OnHideBranchUpdated(FName BranchName, int32 Index, FString OldValue, FString NewValue)
{
	OnHideBranchRemoved(BranchName, Index, OldValue);
	OnHideBranchAdded(BranchName, NewValue);
}

void USmartDialogueGraph::OnBranchRemoved(FName BranchName)
{
	if (UBranchNode* L_Node = GetBranchNodeByName(BranchName))
	{
		RemoveNode(L_Node);
	}	
}
