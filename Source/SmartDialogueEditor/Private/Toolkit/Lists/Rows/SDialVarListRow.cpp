// 


#include "SDialVarListRow.h"

#include "EditorStyleSet.h"
#include "Toolkit/SDialConfigWidget.h"
#include "Widgets/Input/SNumericEntryBox.h"

void SDialVarListRow::Construct(const FArguments& InArgs)
{
	VarKey = InArgs._VarKey;
	VarValue = InArgs._VarValue;
	VarDesc = InArgs._VarDesc;
	OnDeleteButtonClicked = InArgs._OnDeleteButtonClicked;
	OnChanged = InArgs._OnChanged;

	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		  .AutoWidth()
		  .Padding(0.0f, 0.0f, 4.0f, 0.0f)
		[
			SAssignNew(KeyEditableTextBox, SEditableTextBox)
			.MinDesiredWidth(100.0f)
            .Text(FText::FromString(VarKey.Get()))
			.HintText(FText::FromString("variable"))
			.OnTextCommitted_Lambda([this](const FText&, ETextCommit::Type)
			{
				OnChanged.ExecuteIfBound({DescriptionEditableTextBox->GetText().ToString(), KeyEditableTextBox->GetText().ToString(), VarValue.Get()});
			})
		]
		+ SHorizontalBox::Slot()
		  .AutoWidth()
		  .Padding(0.0f, 0.0f, 4.0f, 0.0f)
		[
			SAssignNew(DefaultValueNumericEntryBox, SNumericEntryBox<int32>)
			.MinDesiredValueWidth(80.0f)
			.Value_Lambda( [this] { return VarValue.Get(); } ) 
			.OnValueChanged_Lambda([this](int32 InValue)
			{
				VarValue = InValue;
				OnChanged.ExecuteIfBound({DescriptionEditableTextBox->GetText().ToString(), KeyEditableTextBox->GetText().ToString(), VarValue.Get()});
			})
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SAssignNew(DescriptionEditableTextBox, SEditableTextBox)
			.Text(FText::FromString(VarDesc.Get()))
			.HintText(FText::FromString("description"))
			.OnTextCommitted_Lambda([this](const FText&, ETextCommit::Type)
			{
				OnChanged.ExecuteIfBound({DescriptionEditableTextBox->GetText().ToString(), KeyEditableTextBox->GetText().ToString(), VarValue.Get()});
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
