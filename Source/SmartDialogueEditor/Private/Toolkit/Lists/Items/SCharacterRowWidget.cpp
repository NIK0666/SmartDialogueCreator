// SCharacterRowWidget.cpp

#include "SCharacterRowWidget.h"
#include "EditorStyleSet.h"

void SCharacterRowWidget::Construct(const FArguments& InArgs)
{
	Id = InArgs._Id;
	Name = InArgs._Name;
	OnDeleteButtonClicked = InArgs._OnDeleteButtonClicked;

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(0.4f)
		[
			SNew(SEditableTextBox)
			.HintText(FText::FromString("id"))
			.Text(FText::FromString(Id.Get()))
		]
		+ SHorizontalBox::Slot()
		.FillWidth(0.4f)
		[
			SNew(SEditableTextBox)
			.HintText(FText::FromString("name"))
			.Text(FText::FromString(Name.Get()))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
			.ButtonStyle(FEditorStyle::Get(), "FlatButton.Danger")
			.ForegroundColor(FSlateColor::UseForeground())
			.OnClicked_Lambda([this]()
			{
				OnDeleteButtonClicked.ExecuteIfBound();
				return FReply::Handled();
			})
			[
				SNew(SImage)
				.Image(FEditorStyle::Get().GetBrush("Icons.Cross"))
			]
		]
	];
}