// 


#include "SGlobalVarRow.h"

#include "EditorStyleSet.h"
#include "Toolkit/SDialConfigWidget.h"

void SGlobalVarRow::Construct(const FArguments& InArgs)
{
	VarId = InArgs._VarId;
	ParentWidget = InArgs._ParentWidget;

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
				.OnClicked(this, &SGlobalVarRow::OnDeleteButtonClicked)
			[
				SNew(SImage)
				.Image(FEditorStyle::Get().GetBrush("Icons.Cross"))
			]
		]
	];
}

TSharedPtr<FString> SGlobalVarRow::GetVarId()
{
	return VarId;
}

FReply SGlobalVarRow::OnDeleteButtonClicked()
{
	if (ParentWidget.IsValid())
	{
		ParentWidget.Pin()->OnPublicVarDeleted(VarId);
	}

	return FReply::Handled();
}
