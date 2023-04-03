// SGraphNode_Branch.cpp
#include "SGraphNode_Branch.h"
#include "BranchNode.h"
#include "SGraphPin.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

void SGraphNode_Branch::Construct(const FArguments& InArgs, UBranchNode* InNode)
{
	this->GraphNode = InNode;

	this->SetCursor(EMouseCursor::CardinalCross);
	this->UpdateGraphNode();
}

TSharedRef<SWidget> SGraphNode_Branch::CreateNodeContentArea()
{    
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(2.0f)
		[
			SNew(SInlineEditableTextBlock)
			.Text(FText::FromString(Cast<UBranchNode>(GraphNode)->PhraseText))
			.OnTextCommitted_Lambda([this](const FText& InText, ETextCommit::Type CommitInfo)
			{
				Cast<UBranchNode>(GraphNode)->PhraseText = InText.ToString();
			})
		];
}

TSharedRef<SWidget> SGraphNode_Branch::CreateTitleWidget(TSharedPtr<SNodeTitle> NodeTitle)
{
	return CreateNodeTitleWidget();
}

FText SGraphNode_Branch::GetNodeTitle() const
{
	UBranchNode* BranchNode = Cast<UBranchNode>(GraphNode);
	return BranchNode ? FText::FromString(BranchNode->BranchTitle) : FText::GetEmpty();
}

TSharedRef<SWidget> SGraphNode_Branch::CreateNodeTitleWidget()
{   
	return SNew(SInlineEditableTextBlock)
		.Text(this, &SGraphNode_Branch::GetNodeTitle)
		.Font(FAppStyle::GetFontStyle("BoldFont"))
		.OnTextCommitted_Lambda([this](const FText& InText, ETextCommit::Type CommitInfo)
		{
			if (UBranchNode* BranchNode = Cast<UBranchNode>(GraphNode))
			{
				BranchNode->BranchTitle = InText.ToString();
			}
		})
		.IsReadOnly(!IsEditable.Get());
}

void SGraphNode_Branch::EnterRenameMode()
{
	if (IsEditable.Get())
	{
		TitleSwitcher->SetActiveWidgetIndex(1);
		FSlateApplication::Get().SetKeyboardFocus(TitleEditableText, EFocusCause::Mouse);
	}
}