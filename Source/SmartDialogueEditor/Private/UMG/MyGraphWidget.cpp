// MyGraphWidget.cpp

#include "MyGraphWidget.h"

#include "MyGraphEdge.h"
#include "MyGraphNode.h"

void UMyGraphWidget::AddNode(UMyGraphNode* Node)
{
	GraphNodes.Add(Node);
	// Здесь можно добавить логику отображения новой ноды в UMG интерфейсе
}

void UMyGraphWidget::RemoveNode(UMyGraphNode* Node)
{
	GraphNodes.Remove(Node);
	// Здесь можно добавить логику удаления ноды из UMG интерфейса
}

void UMyGraphWidget::ConnectNodes(UMyGraphNode* NodeA, UMyGraphNode* NodeB)
{
	if (!NodeA || !NodeB)
	{
		return;
	}

	UMyGraphEdge* NewEdge = NewObject<UMyGraphEdge>();
	if (NewEdge)
	{
		NewEdge->NodeA = NodeA;
		NewEdge->NodeB = NodeB;
		GraphEdges.Add(NewEdge);

		// Здесь можно добавить логику отображения новой связи в UMG интерфейсе
	}
}

void UMyGraphWidget::DisconnectNodes(UMyGraphNode* NodeA, UMyGraphNode* NodeB)
{
	if (!NodeA || !NodeB)
	{
		return;
	}

	TArray<UMyGraphEdge*> EdgesToRemove;

	for (UMyGraphEdge* Edge : GraphEdges)
	{
		if ((Edge->NodeA == NodeA && Edge->NodeB == NodeB) || (Edge->NodeA == NodeB && Edge->NodeB == NodeA))
		{
			EdgesToRemove.Add(Edge);

			// Здесь можно добавить логику удаления связи из UMG интерфейса
		}
	}

	for (UMyGraphEdge* Edge : EdgesToRemove)
	{
		GraphEdges.Remove(Edge);
	}
}