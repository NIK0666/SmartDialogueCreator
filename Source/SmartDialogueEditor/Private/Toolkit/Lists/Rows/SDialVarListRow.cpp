// 


#include "SDialVarListRow.h"

#include "EditorStyleSet.h"
#include "Toolkit/SDialConfigWidget.h"

void SDialVarListRow::Construct(const FArguments& InArgs)
{
	VarKey = InArgs._VarKey;
	VarValue = InArgs._VarValue;
	VarDesc = InArgs._VarDesc;
	OnDeleteButtonClicked = InArgs._OnDeleteButtonClicked;

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		  .AutoWidth()
		  .Padding(0.0f, 0.0f, 4.0f, 0.0f)
		[
			SNew(SEditableTextBox)
				.MinDesiredWidth(100.0f)
                .Text(TAttribute<FText>::Create(TAttribute<FText>::FGetter::CreateLambda([this]() { return FText::FromString(VarKey.Get()); })))
				.HintText(FText::FromString("variable"))
		]
		+ SHorizontalBox::Slot()
		  .AutoWidth()
		  .Padding(0.0f, 0.0f, 4.0f, 0.0f)
		[
			SNew(SEditableTextBox)
				.MinDesiredWidth(80.0f)
				.Text(TAttribute<FText>::Create(TAttribute<FText>::FGetter::CreateLambda([this]() { return FText::FromString(VarValue.Get()); })))
				.HintText(FText::FromString("default"))
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SNew(SEditableTextBox)
			.Text(TAttribute<FText>::Create(TAttribute<FText>::FGetter::CreateLambda([this]() { return FText::FromString(VarDesc.Get()); })))
			.HintText(FText::FromString("description"))
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

FString SDialVarListRow::GetVarKey()
{
	return VarKey.Get();
}