// MyGraphWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyGraphWidget.generated.h"

class UMyGraphNode;
class UMyGraphEdge;

UCLASS()
class SMARTDIALOGUECORE_API UMyGraphWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Здесь можно добавить функции и свойства виджета
	UFUNCTION(BlueprintCallable, Category = "Graph")
	void AddNode(UMyGraphNode* Node);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	void RemoveNode(UMyGraphNode* Node);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	void ConnectNodes(UMyGraphNode* NodeA, UMyGraphNode* NodeB);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	void DisconnectNodes(UMyGraphNode* NodeA, UMyGraphNode* NodeB);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Graph")
	TArray<UMyGraphNode*> GraphNodes;

	UPROPERTY(BlueprintReadOnly, Category = "Graph")
	TArray<UMyGraphEdge*> GraphEdges;
};