// BranchNode.cpp


#include "BranchNode.h"

#include "SGraphNode_Branch.h"

UBranchNode::UBranchNode()
{
	BranchTitle = TEXT("Branch Title");
	PhraseText = TEXT("Phrase Text");
}

FText UBranchNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(BranchTitle);
}

void UBranchNode::AllocateDefaultPins()
{
	CreateInputPin();
	CreateOutputPins();
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
