// 


#include "SDialVarListRow.h"

#include "EditorStyleSet.h"
#include "Toolkit/SDialConfigWidget.h"

void SDialVarListRow::Construct(const FArguments& InArgs)
{
	VarId = InArgs._VarId;

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		  .AutoWidth()
		  .Padding(0.0f, 0.0f, 4.0f, 0.0f)
		[
			SNew(SEditableTextBox)
				.MinDesiredWidth(100.0f)
				.HintText(FText::FromString("variable"))
		]
		+ SHorizontalBox::Slot()
		  .AutoWidth()
		  .Padding(0.0f, 0.0f, 4.0f, 0.0f)
		[
			SNew(SEditableTextBox)
				.MinDesiredWidth(80.0f)
				.HintText(FText::FromString("default"))
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SNew(SEditableTextBox)
			.HintText(FText::FromString("description"))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
				.ButtonStyle(FEditorStyle::Get(), "FlatButton.Danger")
				.ForegroundColor(FSlateColor::UseForeground())
				.OnClicked(this, &SDialVarListRow::OnDeleteButtonClicked)
			[
				SNew(SImage)
				.Image(FEditorStyle::Get().GetBrush("Icons.Cross"))
			]
		]
	];
}

TSharedPtr<FString> SDialVarListRow::GetVarId()
{
	return VarId;
}

FReply SDialVarListRow::OnDeleteButtonClicked()
{
	return FReply::Handled();
}