// 


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

TSharedPtr<SGraphNode> UBranchNode::CreateVisualWidget()
{
	return SNew(SGraphNode_Branch, this);
}
