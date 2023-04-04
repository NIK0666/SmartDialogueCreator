// BranchNode.h
#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "BranchNode.generated.h"

UCLASS()
class SMARTDIALOGUEEDITOR_API UBranchNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UBranchNode();
	
	UPROPERTY(EditAnywhere, Category = "Branch")
	FString BranchTitle;

	UPROPERTY(EditAnywhere, Category = "Branch")
	FString PhraseText;
	
	virtual void AllocateDefaultPins() override;

protected:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;

	void CreateInputPin();
	void CreateOutputPins();
};
