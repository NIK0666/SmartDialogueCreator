// 


#include "SDialVarRow.h"

#include "EditorStyleSet.h"
#include "Toolkit/SDialConfigWidget.h"

void SDialVarRow::Construct(const FArguments& InArgs)
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
				.OnClicked(this, &SDialVarRow::OnDeleteButtonClicked)
			[
				SNew(SImage)
				.Image(FEditorStyle::Get().GetBrush("Icons.Cross"))
			]
		]
	];
}

TSharedPtr<FString> SDialVarRow::GetVarId()
{
	return VarId;
}

FReply SDialVarRow::OnDeleteButtonClicked()
{
	return FReply::Handled();
}