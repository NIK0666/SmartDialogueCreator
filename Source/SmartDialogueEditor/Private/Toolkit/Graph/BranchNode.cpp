// BranchNode.cpp


#include "BranchNode.h"

#include "SGraphNode_Branch.h"
#include "SmartDialogue.h"
#include "Toolkit/FSmartDialogueEditor.h"



FText UBranchNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromName(BranchName);
}

UBranchNode::~UBranchNode()
{
	if (Editor)
	{
		Editor->GetDialogue()->OnBranchRenamed.Remove(BranchRenamedHandle);
	}
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
	return SNew(SGraphNode_Branch, this);
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
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Else);
}

FSmartDialogueBranch* UBranchNode::GetBranchPtr()
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
	if (Editor->GetDialogue()->RenameBranch(BranchName, NewName))
	{
		BranchName = NewName;
	}
	else
	{
		// Вернуть старое имя, если переименование не удалось
		// Здесь вы можете добавить код для обновления пользовательского интерфейса
	}
}