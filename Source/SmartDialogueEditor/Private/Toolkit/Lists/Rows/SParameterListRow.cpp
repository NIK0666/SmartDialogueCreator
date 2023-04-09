// SParameterListRow.cpp

#include "SParameterListRow.h"

void SParameterListRow::Construct(const FArguments& InArgs)
{
	Parameter = InArgs._Parameter;
	Desc = InArgs._Desc;
	OnDeleteButtonClicked = InArgs._OnDeleteButtonClicked;

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(0.4f)
		[
			SNew(SEditableTextBox)
			.HintText(FText::FromString("Parameter"))
			.Text(FText::FromString(Parameter.Get()))
		]
		+ SHorizontalBox::Slot()
		.FillWidth(0.4f)
		[
			SNew(SEditableTextBox)
			.HintText(FText::FromString("Description"))
			.Text(FText::FromString(Desc.Get()))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.ForegroundColor(FSlateColor::UseForeground())
			.OnClicked_Lambda([this]()
			{
				OnDeleteButtonClicked.ExecuteIfBound();
				return FReply::Handled();
			})
			[
				SNew(SImage)
				.Image(FAppStyle::Get().GetBrush("Cross"))
			]
		]
	];
}

