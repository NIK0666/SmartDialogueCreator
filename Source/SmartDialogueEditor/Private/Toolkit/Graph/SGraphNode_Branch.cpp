// SGraphNode_Branch.cpp
#include "SGraphNode_Branch.h"
#include "BranchNode.h"
#include "SBranchPin.h"
#include "SGraphPin.h"
#include "SmartDialogueData.h"
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
	UBranchNode* BranchNode = Cast<UBranchNode>(GraphNode);

	return SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SGraphNode::CreateNodeContentArea()
		]
		+ SOverlay::Slot()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(32.f, 8.f)
			[
				SNew(SBox)
				.Padding(0.f)
				.HeightOverride(32.f)
				[
					SNew(SInlineEditableTextBlock)
					.Text_Lambda([BranchNode]() { return BranchNode->GetBranchPtr()->Text; })
					.OnTextCommitted_Lambda([BranchNode](const FText& InText, ETextCommit::Type CommitInfo)
					{
						BranchNode->GetBranchPtr()->Text = InText;
					})
					.WrapTextAt(MaxNodeWidth)
					.MultiLine(true)
				]
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
	return BranchNode ? FText::FromName(BranchNode->GetBranchName()) : FText::GetEmpty();
}

TSharedRef<SWidget> SGraphNode_Branch::CreateNodeTitleWidget()
{   
	return SNew(SBox)
		.Padding(0.f)
		.MaxDesiredWidth(MaxNodeWidth)
		[
			SNew(SInlineEditableTextBlock)
			.Text(this, &SGraphNode_Branch::GetNodeTitle)
			.Font(FAppStyle::GetFontStyle("BoldFont"))
			.OnTextCommitted_Lambda([this](const FText& InText, ETextCommit::Type CommitInfo)
			{
				UBranchNode* BranchNode = Cast<UBranchNode>(GraphNode);
				BranchNode->RenameBranch(InText.ToString());
			})
			.IsReadOnly(!IsEditable.Get())
		];
}

TSharedPtr<SGraphPin> SGraphNode_Branch::CreatePinWidget(UEdGraphPin* Pin) const
{
	return SNew(SBranchPin, Pin);
}