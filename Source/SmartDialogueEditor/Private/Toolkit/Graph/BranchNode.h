// BranchNode.h
#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "BranchNode.generated.h"

class FSmartDialogueEditor;
UCLASS()
class SMARTDIALOGUEEDITOR_API UBranchNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UBranchNode();
	
	UPROPERTY(EditAnywhere, Category = "Branch")
	FString PhraseText;
	
	virtual void AllocateDefaultPins() override;

	UBranchNode* Initialize(const FName& InBranchName, FSmartDialogueEditor* InEditor);
	FName GetBranchName() const { return BranchName; }

protected:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;

	void CreateInputPin();
	void CreateOutputPins();
	
	FName BranchName;
	FSmartDialogueEditor* Editor;
};
