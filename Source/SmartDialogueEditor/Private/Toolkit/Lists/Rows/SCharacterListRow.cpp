// SCharacterListRow.cpp

#include "SCharacterListRow.h"
#include "EditorStyleSet.h"

void SCharacterListRow::Construct(const FArguments& InArgs)
{
	Id = InArgs._Id;
	Name = InArgs._Name;
	OnDeleteButtonClicked = InArgs._OnDeleteButtonClicked;
	OnChanged = InArgs._OnChanged;
	
	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(0.4f)
		[
			SAssignNew(IdEditableTextBox, SEditableTextBox)
			.HintText(FText::FromString("id"))
			.Text(FText::FromString(Id.Get()))
			.OnTextCommitted_Lambda([this](const FText&, ETextCommit::Type)
			{
				OnChanged.ExecuteIfBound({IdEditableTextBox->GetText().ToString(), NameEditableTextBox->GetText()});
			})
		]
		+ SHorizontalBox::Slot()
		.FillWidth(0.4f)
		[
			SAssignNew(NameEditableTextBox, SEditableTextBox)
			.HintText(FText::FromString("name"))
			.Text(Name.Get())
			.OnTextCommitted_Lambda([this](const FText&, ETextCommit::Type)
			{
				OnChanged.ExecuteIfBound({IdEditableTextBox->GetText().ToString(), NameEditableTextBox->GetText()});
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