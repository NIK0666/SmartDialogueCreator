// SmartDialogueGraph.h

#pragma once

#include "CoreMinimal.h"
#include "SmartDialogueData.h"
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
	void LoadNodesFromAsset();
	FSmartDialogueEditor* GetEditor() const { return Editor; }

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	UBranchNode* CreateBranchNode(const FName& BranchName) const;
	void AddBranchNode(FSmartDialogueBranch& SmartDialogueBranch);
	FString GetNodesInformation();

	FVector2D LastNodePos;
	bool bIsInitializeGraph = false;

private:
	void CreateConnections();
	void SortNodes();
	UBranchNode* GetBranchNodeByName(FName BranchName) const;

	FSmartDialogueEditor* Editor;
};