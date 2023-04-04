// SmartDialogueGraph.h

#pragma once

#include "CoreMinimal.h"
#include "BranchNode.h"
#include "EdGraph/EdGraph.h"
#include "SmartDialogueGraph.generated.h"

class FSmartDialogueEditor;

UCLASS()
class SMARTDIALOGUEEDITOR_API USmartDialogueGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	USmartDialogueGraph();

	void SetEditor(FSmartDialogueEditor* InEditor);

private:
	FSmartDialogueEditor* Editor;
};