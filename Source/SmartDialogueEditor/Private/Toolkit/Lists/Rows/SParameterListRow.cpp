// SParameterListRow.cpp

#include "SParameterListRow.h"

void SParameterListRow::Construct(const FArguments& InArgs)
{
	Parameter = InArgs._Parameter;
	Desc = InArgs._Desc;
	OnDeleteButtonClicked = InArgs._OnDeleteButtonClicked;
	OnChanged = InArgs._OnChanged;
	
	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(0.4f)
		[
			SAssignNew(ParameterEditableTextBox, SEditableTextBox)
			.HintText(FText::FromString("Parameter"))
			.Text(FText::FromString(Parameter.Get()))
			.OnTextCommitted_Lambda([this](const FText&, ETextCommit::Type)
			{
				OnChanged.ExecuteIfBound({DescriptionEditableTextBox->GetText().ToString(), ParameterEditableTextBox->GetText().ToString()});
			})
		]
		+ SHorizontalBox::Slot()
		.FillWidth(0.4f)
		[
			SAssignNew(DescriptionEditableTextBox, SEditableTextBox)
			.HintText(FText::FromString("Description"))
			.Text(FText::FromString(Desc.Get()))
			.OnTextCommitted_Lambda([this](const FText&, ETextCommit::Type)
			{
				OnChanged.ExecuteIfBound({DescriptionEditableTextBox->GetText().ToString(), ParameterEditableTextBox->GetText().ToString()});
			})
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

