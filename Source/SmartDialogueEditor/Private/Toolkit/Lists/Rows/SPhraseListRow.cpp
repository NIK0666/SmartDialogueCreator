// SPhraseListRow.cpp


#include "SPhraseListRow.h"

#include "EditorStyleSet.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"


void SPhraseListRow::Construct(const FArguments& InArgs)
{

	// Заполните массивы с опциями для комбо-боксов по вашим данным
	CharacterOptions.Add(MakeShareable(new FString(TEXT("Option1"))));
	CharacterOptions.Add(MakeShareable(new FString(TEXT("Option2"))));

	VarOptions.Add(MakeShareable(new FString(TEXT("Var1"))));
	VarOptions.Add(MakeShareable(new FString(TEXT("Var2"))));

	ComparisonOptions.Add(MakeShareable(new FString(TEXT("=="))));
	ComparisonOptions.Add(MakeShareable(new FString(TEXT("!="))));

	
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(CharacterComboBox, SComboBox<TSharedPtr<FString>>)
				.ContentPadding(FMargin(2.0f))
				.OptionsSource(&CharacterOptions)
				.OnGenerateWidget(this, &SPhraseListRow::GenerateCharacterOption)
				.OnSelectionChanged(this, &SPhraseListRow::OnCharacterSelected)
				.InitiallySelectedItem(CharacterOptions[0])
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("SPhraseListRow", "Character", "[character]"))
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SEditableTextBox)
				.HintText(NSLOCTEXT("SPhraseListRow", "Animation", "Animation"))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(VarComboBox, SComboBox<TSharedPtr<FString>>)
				.ContentPadding(FMargin(2.0f))
				.OptionsSource(&VarOptions)
				.OnGenerateWidget(this, &SPhraseListRow::GenerateVarOption)
				.OnSelectionChanged(this, &SPhraseListRow::OnVarSelected)
				.InitiallySelectedItem(VarOptions[0])
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("SPhraseListRow", "Var", "[Var]"))
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(ComparisonComboBox, SComboBox<TSharedPtr<FString>>)
				.ContentPadding(FMargin(2.0f))
				.OptionsSource(&ComparisonOptions)
				.OnGenerateWidget(this, &SPhraseListRow::GenerateComparisonOption)
				.OnSelectionChanged(this, &SPhraseListRow::OnComparisonSelected)
				.InitiallySelectedItem(ComparisonOptions[0])
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("SPhraseListRow", "Comparison", "=="))
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SEditableTextBox)
				.HintText(NSLOCTEXT("SPhraseListRow", "Value", "Value"))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged(this, &SPhraseListRow::OnOrCheckStateChanged)
				.Content()
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("SPhraseListRow", "Or", "Or?"))
				]
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(SBox)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Content()
				[
					SNew(SImage)
					.Image(FEditorStyle::GetBrush("Icons.Settings"))
				]
				.OnClicked(this, &SPhraseListRow::OnSettingsButtonClicked)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Content()
				[
					SNew(SImage)
					.Image(FAppStyle::GetBrush("SoftwareCursor_Grab"))
				]
				.OnClicked(this, &SPhraseListRow::OnHandButtonClicked)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Content()
				[
					SNew(SImage)
					.Image(FEditorStyle::GetBrush("Cross"))
				]
				.OnClicked(this, &SPhraseListRow::OnDeleteButtonClicked)
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.WidthOverride(300.0f)
			[
				SNew(SMultiLineEditableTextBox)
			.AutoWrapText(true)
			.OnTextChanged(this, &SPhraseListRow::OnMultiLineTextChanged)
			// .MinDesiredWidth(300.0f)
			]
		]
	];
}

TSharedRef<SWidget> SPhraseListRow::GenerateCharacterOption(TSharedPtr<FString> Option)
{
	return SNew(STextBlock).Text(FText::FromString(*Option));
}

TSharedRef<SWidget> SPhraseListRow::GenerateVarOption(TSharedPtr<FString> Option)
{
	return SNew(STextBlock).Text(FText::FromString(*Option));
}

TSharedRef<SWidget> SPhraseListRow::GenerateComparisonOption(TSharedPtr<FString> Option)
{
	return SNew(STextBlock).Text(FText::FromString(*Option));
}

void SPhraseListRow::OnCharacterSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	// Обработка выбора элемента из списка Character
}

void SPhraseListRow::OnVarSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	// Обработка выбора элемента из списка Var
}

void SPhraseListRow::OnComparisonSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	// из списка Comparison
}

void SPhraseListRow::OnOrCheckStateChanged(ECheckBoxState NewState)
{
	// Обработка изменения состояния чекбокса "Or?"
}

FReply SPhraseListRow::OnSettingsButtonClicked()
{
	// Обработка нажатия на кнопку настроек
	return FReply::Handled();
}

FReply SPhraseListRow::OnHandButtonClicked()
{
	// Обработка нажатия на кнопку с иконкой руки
	return FReply::Handled();
}

FReply SPhraseListRow::OnDeleteButtonClicked()
{
	// Обработка нажатия на кнопку удаления
	return FReply::Handled();
}

void SPhraseListRow::OnMultiLineTextChanged(const FText& InText)
{
	// Обработка изменения текста в поле ввода с автоматическим расширением вниз
}