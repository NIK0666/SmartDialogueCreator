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
                    .Text_Lambda([this, BranchNode]()
                    {
                    	if (BranchNode->GetBranchPtr() == nullptr)
                    	{
                    		return FText::GetEmpty();
                    	}
                    	
                        if (!bIsEditing && BranchNode->GetBranchPtr()->Text.IsEmpty())
                        {
                            if (BranchNode->GetBranchPtr()->Phrases.Num() > 0)
                            {
                                return BranchNode->GetBranchPtr()->Phrases[0].Text;
                            }
                            else
                            {
                                return FText::FromString(TEXT("Empty Phrases"));
                            }
                        }
                        return BranchNode->GetBranchPtr()->Text;
                    })                    
                    .ColorAndOpacity_Lambda([this, BranchNode]() -> FSlateColor
                    {
                    	if (BranchNode->GetBranchPtr() == nullptr)
                    	{
							return FLinearColor::Transparent;
						}
                        if (!bIsEditing && BranchNode->GetBranchPtr()->Text.IsEmpty())
                        {
                            return FSlateColor(FLinearColor(1.f, 1.f, 1.f, 0.5f));
                        }
                        return FSlateColor::UseForeground();
                    })
                    .OnTextCommitted_Lambda([this, BranchNode](const FText& InText, ETextCommit::Type CommitInfo)
                    {
                    	if (BranchNode->GetBranchPtr() == nullptr)
                    	{
							return;
						}
                        BranchNode->GetBranchPtr()->Text = InText;
                    	bIsEditing = false;
                    })
                    .WrapTextAt(MaxNodeWidth)
                    .MultiLine(true)
                    .OnEnterEditingMode_Lambda([this]()
                    {
                        bIsEditing = true;
                    })
                    .OnExitEditingMode_Lambda([this]()
                    {
                        bIsEditing = false;
                    })
                    .ModiferKeyForNewLine(EModifierKey::Shift) // Установка модификатора клавиши для создания новой строки на Shift
                ]
            ]
            + SVerticalBox::Slot()
		    .AutoHeight()
		    .Padding(16.f, 8.f, 32.f, 8.f)
		    [
		        SNew(SBox)
		        .Padding(0.f)
		        .HeightOverride(32.f)
		        .Visibility_Lambda([this, BranchNode]() -> EVisibility
		        {
		            if (BranchNode->GetBranchPtr() == nullptr || BranchNode->GetBranchPtr()->Phrases.Num() <= 1)
		            {
		                return EVisibility::Collapsed;
		            }
		            return EVisibility::Visible;
		        })
		        [
		            SNew(STextBlock)
		            .Text_Lambda([this, BranchNode]()
		            {
		                if (BranchNode->GetBranchPtr() == nullptr)
		                {
		                    return FText::GetEmpty();
		                }
		                return BranchNode->GetBranchPtr()->Phrases.Last().Text;
		            })
		            .WrapTextAt(MaxNodeWidth)
		            .AutoWrapText(true)
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