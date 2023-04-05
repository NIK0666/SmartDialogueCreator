// BranchNode.h
#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "BranchNode.generated.h"

struct FSmartDialogueBranch;
class FSmartDialogueEditor;
UCLASS()
class SMARTDIALOGUEEDITOR_API UBranchNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	virtual ~UBranchNode() override;
	
	virtual void AllocateDefaultPins() override;

	UBranchNode* Initialize(const FName& InBranchName, FSmartDialogueEditor* InEditor);
	FName GetBranchName() const { return BranchName; }

	FSmartDialogueBranch* GetBranchPtr();
	void RenameBranch(const FString& NewNameString);

	TSharedPtr<SGraphNode> GetVisualWidget() { return VisualWidget; }

protected:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	
	UFUNCTION()
	void OnBranchRenamed(FName OldName, FName NewName);

	void CreateInputPin();
	void CreateOutputPins();
	
	FName BranchName;
	FSmartDialogueEditor* Editor;

	FDelegateHandle BranchRenamedHandle;
	TSharedPtr<SGraphNode> VisualWidget;
};
