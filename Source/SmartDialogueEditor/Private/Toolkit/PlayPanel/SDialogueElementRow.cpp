// SDialogueElementRow.cpp
#include "SDialogueElementRow.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

void SDialogueElementRow::Construct(const FArguments& InArgs)
{
	const FDialogueElementData& DialogueElementData = InArgs._DialogueElementData;

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.Padding(16, 0, 16, 0)
		.AutoHeight()
		[
			SNew(SBorder)
			.Padding(FMargin(0, 4, 0, 4))
			.BorderImage(FCoreStyle::Get().GetBrush("NoBorder"))
			.Content()
			[
				SNew(STextBlock)
				.Text(FText::FromString(DialogueElementData.Title))
				.ColorAndOpacity(DialogueElementData.TitleColor)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 14))
				.Visibility(DialogueElementData.bIsTitleVisible ? EVisibility::Visible : EVisibility::Collapsed)
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.Padding(FMargin(16, 0, 0, 8))
			[
				SNew(STextBlock)
				.Text(DialogueElementData.Content)
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 12))
				.WrapTextAt(500)
				.ColorAndOpacity(DialogueElementData.ContentColor)
				.Visibility(DialogueElementData.bIsContentVisible ? EVisibility::Visible : EVisibility::Collapsed)
			]
		]
	];
}