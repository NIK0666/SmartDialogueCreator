// BranchNode.cpp


#include "BranchNode.h"

#include "SGraphNode_Branch.h"
#include "SmartDialogue.h"
#include "Helpers/EditorDataHelper.h"
#include "Toolkit/FSmartDialogueEditor.h"


FVector2D UBranchNode::GetNodeSize()
{
	return VisualWidget->GetCachedGeometry().Size;
}

FLinearColor UBranchNode::GetNodeTitleColor() const
{
	if (GetBranchPtr() == nullptr)
	{
		return FLinearColor::Transparent;
	}
	
    if (GetBranchPtr()->Closed)
    {
        return FLinearColor::Yellow;
    }
    
    if (!GetBranchPtr()->Event.Name.IsEmpty())
    {
        return FLinearColor::Red;
    }
    
    if (FindPin(UEdGraphSchema_K2::PN_Execute)->LinkedTo.Num() == 0)
    {
        return FLinearColor::Green;
    }
	
	return Super::GetNodeTitleColor();
}

FText UBranchNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromName(BranchName);
}

UBranchNode::~UBranchNode()
{
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
	
	BranchRenamedHandle = Editor->GetDialogue()->OnBranchRenamed.AddUObject(this, &UBranchNode::OnBranchRenamed);

	return this;
}

TSharedPtr<SGraphNode> UBranchNode::CreateVisualWidget()
{
	VisualWidget = SNew(SGraphNode_Branch, this);
	return VisualWidget;
}

void UBranchNode::OnBranchRenamed(FName OldName, FName NewName)
{
	if (BranchName == OldName)
	{
		BranchName = NewName;
	}
}

void UBranchNode::CreateInputPin()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
}

void UBranchNode::CreateOutputPins()
{
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, FName(""));
}

FSmartDialogueBranch* UBranchNode::GetBranchPtr() const
{
	if (auto BranchPtr = Editor->GetBranch(BranchName))
	{
		return BranchPtr;
	}

	return nullptr;
}

void UBranchNode::RenameBranch(const FString& NewNameString)
{
	const FName NewName = FName(NewNameString);
	if (UEditorDataHelper::RenameBranch(Editor, BranchName, NewName))
	{
		BranchName = NewName;
	}
}