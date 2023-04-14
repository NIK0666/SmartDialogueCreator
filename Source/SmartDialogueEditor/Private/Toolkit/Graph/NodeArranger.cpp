
// UNodeArranger.cpp
#include "NodeArranger.h"

#include "BranchNode.h"

TMap<UEdGraphNode*, FVector2D> UNodeArranger::ArrangeNodes(const TArray<UEdGraphNode*>& Nodes)
{
	TMap<UEdGraphNode*, FVector2D> NodePositions;
	TArray<UEdGraphNode*> NodesWithoutInputs;
	int32 LastY = 0;

	// Find nodes without inputs
	for (auto Node : Nodes)
	{
		if (!GetParent(Node))
		{
			NodesWithoutInputs.Add(Node);
		}
	}

	for (auto Node : NodesWithoutInputs)
	{
		TArray<UEdGraphNode*> NodeQueue;
		NodeQueue.Add(Node);
		
		TSet<UEdGraphNode*> ProcessedNodes;
		while (NodeQueue.Num() > 0)
		{
			UEdGraphNode* CurrentNode = NodeQueue[0];
			NodeQueue.RemoveAt(0);
			ProcessedNodes.Add(CurrentNode);
			TArray<UEdGraphNode*> Children = GetChildren(CurrentNode);

			if (!NodePositions.Contains(CurrentNode))
			{
				FVector2D Position;

				if (NodesWithoutInputs.IndexOfByKey(CurrentNode) == 0)
				{
					Position = FVector2D(0, 0);
				}
				else
				{
					Position = FVector2D(0, LastY + 1);
					LastY += 1;
				}
				UE_LOG(LogTemp, Log, TEXT("Arrange Root  Node: %s %d %d"), *Cast<UBranchNode>(CurrentNode)->GetBranchName().ToString(), int32(Position.X), int32(Position.Y));
				Position = FindFreePosition(NodePositions, Position);
				NodePositions.Add(CurrentNode, Position);
			}

			int32 offsetY = 0;

			for (int32 i = 0; i < Children.Num(); ++i)
			{
				UEdGraphNode* Child = Children[i];
				FVector2D ParentPosition = NodePositions[CurrentNode];

				// Если позиция узла еще не задана, добавляем ее и увеличиваем счетчик смещения
				if (!NodePositions.Contains(Child))
				{
					FVector2D ChildPosition(ParentPosition.X + 1, ParentPosition.Y + offsetY);
					UE_LOG(LogTemp, Log, TEXT("Arrange Child Node: %s %d %d"), *Cast<UBranchNode>(Child)->GetBranchName().ToString(), int32(ChildPosition.X), int32(ChildPosition.Y));
					ChildPosition = FindFreePosition(NodePositions, ChildPosition);
					NodePositions.Add(Child, ChildPosition);
					offsetY++;
				}

				if (!ProcessedNodes.Contains(Child))
				{
					NodeQueue.Add(Child);
				}
			}
		}
	}

	// FixCollisions(NodePositions);  // вызов функции для исправления коллизий (шаги 6.1-6.7)

	
	return NodePositions;
}

FVector2D UNodeArranger::FindFreePosition(TMap<UEdGraphNode*, FVector2D>& NodePositions, FVector2D DesiredPosition)
{
	FVector2D NewPosition = DesiredPosition;
	while (NodePositions.FilterByPredicate([&](const TPair<UEdGraphNode*, FVector2D>& Entry) { return Entry.Value == NewPosition; }).Num() > 0)
	{
		NewPosition.Y += 1;
	}
	return NewPosition;
}


void UNodeArranger::FixCollisions(TMap<UEdGraphNode*, FVector2D>& NodePositions)
{
	bool bHasCollisions = true;
	TSet<UEdGraphNode*> ShiftedNodes;

	while (bHasCollisions)
	{
		bHasCollisions = false;
		ShiftedNodes.Empty();

		// Проверяем наличие коллизий
		for (auto& Pair : NodePositions)
		{
			UEdGraphNode* Node = Pair.Key;
			FVector2D Position = Pair.Value;

			TArray<UEdGraphNode*> CollidingNodes;
			for (auto& OtherPair : NodePositions)
			{
				if (Node != OtherPair.Key && Position == OtherPair.Value)
				{
					CollidingNodes.Add(OtherPair.Key);
				}
			}

			// Если найдена коллизия, исправляем позиции
			if (CollidingNodes.Num() > 0)
			{
				bHasCollisions = true;

				// Находим родительские узлы и сортируем их по Y
				TArray<UEdGraphNode*> Parents;
				if (auto FirstParent = GetParent(Node))
				{
					Parents.Add(FirstParent);
				}
				else
				{
					Parents.Add(Node);
				}
				

				for (UEdGraphNode* CollidingNode : CollidingNodes)
				{
					if (auto ParentNode = GetParent(CollidingNode))
					{
						Parents.Add(ParentNode);
					}
					else
					{
						Parents.Add(CollidingNode);
					}
					
				}

				Parents.Sort([&](UEdGraphNode& A, UEdGraphNode& B)
				{
					return NodePositions[&A].Y < NodePositions[&B].Y;
				});

				// Смещаем узлы вниз
				for (int32 i = 1; i < Parents.Num(); ++i)
				{
					UEdGraphNode* Parent = Parents[i];
					ShiftNodeAndChildren(Parent, FVector2D(0, 1), NodePositions, ShiftedNodes);
				}
			}
		}
	}
}

void UNodeArranger::ShiftNodeAndChildren(UEdGraphNode* Node, FVector2D ShiftValue, TMap<UEdGraphNode*, FVector2D>& NodePositions, TSet<UEdGraphNode*>& ShiftedNodes)
{
	if (ShiftedNodes.Contains(Node))
	{
		return;
	}

	NodePositions[Node] += ShiftValue;
	ShiftedNodes.Add(Node);
	UE_LOG(LogTemp, Log, TEXT("ShiftNodeAndChildren %s"), *Node->GetName())

	TArray<UEdGraphNode*> Children = GetChildren(Node);
	for (UEdGraphNode* Child : Children)
	{
		ShiftNodeAndChildren(Child, ShiftValue, NodePositions, ShiftedNodes);
	}
}



TArray<UEdGraphNode*> UNodeArranger::GetChildren(UEdGraphNode* CurrentNode)
{
	TArray<UEdGraphNode*> Result;
	
	UEdGraphPin* InputPin = CurrentNode->FindPin(UEdGraphSchema_K2::PN_Then);
	for (auto Element : InputPin->LinkedTo)
	{
		if (Element->PinName == UEdGraphSchema_K2::PN_Execute)
		{
			Result.Add(Element->GetOwningNode());
		}
	}
	return Result;
}

UEdGraphNode* UNodeArranger::GetParent(UEdGraphNode* CurrentNode)
{
	UEdGraphPin* OutputPin = CurrentNode->FindPin(UEdGraphSchema_K2::PN_Execute);
	for (auto Element : OutputPin->LinkedTo)
	{
		if (Element->PinName == UEdGraphSchema_K2::PN_Then)
		{
			return Element->GetOwningNode();
		}
	}

	return nullptr;

}