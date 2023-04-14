// NodeArranger.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EdGraph/EdGraphNode.h"
#include "NodeArranger.generated.h"

UCLASS()
class UNodeArranger : public UObject
{
	GENERATED_BODY()

public:
	TMap<UEdGraphNode*, FVector2D> ArrangeNodes(const TArray<UEdGraphNode*>& Nodes);

private:
	FVector2D FindFreePosition(TMap<UEdGraphNode*, FVector2D>& NodePositions, FVector2D DesiredPosition);
	void FixCollisions(TMap<UEdGraphNode*, FVector2D>& NodePositions);
	void ShiftNodeAndChildren(UEdGraphNode* Node, FVector2D ShiftValue, TMap<UEdGraphNode*, FVector2D>& NodePositions, TSet<UEdGraphNode*>& ShiftedNodes);
	TArray<UEdGraphNode*> GetChildren(UEdGraphNode* CurrentNode);
	UEdGraphNode* GetParent(UEdGraphNode* CurrentNode);
};
