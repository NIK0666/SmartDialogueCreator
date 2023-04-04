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

void SGraphNode_Branch::CreatePinWidgets()
{
	// Create Pin widgets for each of the pins.
	for (int32 PinIndex = 0; PinIndex < GraphNode->Pins.Num(); ++PinIndex)
	{
		UEdGraphPin* CurPin = GraphNode->Pins[PinIndex];
		CurPin->PinName = FName();
	}

	SGraphNode::CreatePinWidgets();
}

TSharedRef<SWidget> SGraphNode_Branch::CreateNodeContentArea()
{
	return SNew(SOverlay)
	+SOverlay::Slot()
	[
		SGraphNode::CreateNodeContentArea()
	]
	+SOverlay::Slot()
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(32.f, 8.f)
		[
			SNew(SInlineEditableTextBlock)
			.Text(FText::FromString(Cast<UBranchNode>(GraphNode)->PhraseText))
			.OnTextCommitted_Lambda([this](const FText& InText, ETextCommit::Type CommitInfo)
			{
				Cast<UBranchNode>(GraphNode)->PhraseText = InText.ToString();
			})
		]
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