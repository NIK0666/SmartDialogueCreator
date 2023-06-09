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
	UBranchNode* CreateBranchNode(const FName& BranchName);
	void AddBranchNode(FSmartDialogueBranch& SmartDialogueBranch);
	FString GetNodesInformation();

	void OnUserConnectPins(UEdGraphPin* A, UEdGraphPin* B);
	void OnUserDisconnectPins(UEdGraphPin* A, UEdGraphPin* B);
	void HandleSelectedNodesChanged(const TSet<UObject*>& NewSelection);

	bool FindCollisions(const TMap<UEdGraphNode*, FVector2D>& NodePositions);
	void ArrangeNodes();

	FVector2D LastNodePos;
	UEdGraphPin* LastFromPin = nullptr;
	
	bool bIsInitializeGraph = false;

protected:	
	UFUNCTION()
	void OnShowBranchRemoved(FName BranchName, int32 Index, FString Value);
	UFUNCTION()
	void OnShowBranchAdded(FName BranchName, FString String);
	UFUNCTION()
	void OnHideBranchRemoved(FName BranchName, int32 Index, FString Value);
	UFUNCTION()
	void OnHideBranchAdded(FName BranchName, FString String);
	UFUNCTION()
	void OnShowBranchUpdated(FName BranchName, int32 Index, FString OldValue, FString NewValue);
	UFUNCTION()
	void OnHideBranchUpdated(FName BranchName, int32 Index, FString OldValue, FString NewValue);
	UFUNCTION()
	void OnBranchRemoved(FName BranchName);

	
private:
	void SetNodePos(UEdGraphNode* Node, const FVector2D& Position);

	void CreateConnections();

	UBranchNode* GetBranchNodeByName(FName BranchName) const;
	void ClearGraph();

	FSmartDialogueEditor* Editor;
};
