// BranchNode.cpp


#include "BranchNode.h"

#include "SGraphNode_Branch.h"
#include "Toolkit/FSmartDialogueEditor.h"


UBranchNode::UBranchNode()
{
	// BranchName = InBranchName;
	// Editor = InEditor;
	// if (Editor->GetSelectedBranch())
	// {
	// 	PhraseText = Editor->GetSelectedBranch()->Text.ToString();
	// }
	// else
	// {
	// 	PhraseText = "Base Phrase Text";
	// }
}

FText UBranchNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromName(BranchName);
}

void UBranchNode::AllocateDefaultPins()
{
	CreateInputPin();
	CreateOutputPins();
}

UBranchNode* UBranchNode::Initialize(const FName& InBranchName, FSmartDialogueEditor* InEditor)
{
	BranchName = InBranchName;
	Editor = InEditor;
	if (FSmartDialogueBranch* BranchPtr = Editor->GetBranch(BranchName))
	{
		PhraseText = BranchPtr->Text.ToString();
	}
	else
	{
		PhraseText = "Base Phrase Text";
	}

	return this;
}

TSharedPtr<SGraphNode> UBranchNode::CreateVisualWidget()
{
	return SNew(SGraphNode_Branch, this);
}

void UBranchNode::CreateInputPin()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
}

void UBranchNode::CreateOutputPins()
{
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Else);
}
