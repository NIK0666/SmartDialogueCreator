// SPhraseListItemWidget.cpp


#include "SPhraseListItemWidget.h"

#include "EditorStyleSet.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"


void SPhraseListItemWidget::Construct(const FArguments& InArgs)
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
				.OnGenerateWidget(this, &SPhraseListItemWidget::GenerateCharacterOption)
				.OnSelectionChanged(this, &SPhraseListItemWidget::OnCharacterSelected)
				.InitiallySelectedItem(CharacterOptions[0])
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("SPhraseListItemWidget", "Character", "[character]"))
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SEditableTextBox)
				.HintText(NSLOCTEXT("SPhraseListItemWidget", "Animation", "Animation"))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(VarComboBox, SComboBox<TSharedPtr<FString>>)
				.ContentPadding(FMargin(2.0f))
				.OptionsSource(&VarOptions)
				.OnGenerateWidget(this, &SPhraseListItemWidget::GenerateVarOption)
				.OnSelectionChanged(this, &SPhraseListItemWidget::OnVarSelected)
				.InitiallySelectedItem(VarOptions[0])
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("SPhraseListItemWidget", "Var", "[Var]"))
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(ComparisonComboBox, SComboBox<TSharedPtr<FString>>)
				.ContentPadding(FMargin(2.0f))
				.OptionsSource(&ComparisonOptions)
				.OnGenerateWidget(this, &SPhraseListItemWidget::GenerateComparisonOption)
				.OnSelectionChanged(this, &SPhraseListItemWidget::OnComparisonSelected)
				.InitiallySelectedItem(ComparisonOptions[0])
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("SPhraseListItemWidget", "Comparison", "=="))
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SEditableTextBox)
				.HintText(NSLOCTEXT("SPhraseListItemWidget", "Value", "Value"))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged(this, &SPhraseListItemWidget::OnOrCheckStateChanged)
				.Content()
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("SPhraseListItemWidget", "Or", "Or?"))
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
				.OnClicked(this, &SPhraseListItemWidget::OnSettingsButtonClicked)
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
				.OnClicked(this, &SPhraseListItemWidget::OnHandButtonClicked)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Content()
				[
					SNew(SImage)
					.Image(FEditorStyle::GetBrush("Icons.Cross"))
				]
				.OnClicked(this, &SPhraseListItemWidget::OnDeleteButtonClicked)
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
			.OnTextChanged(this, &SPhraseListItemWidget::OnMultiLineTextChanged)
			// .MinDesiredWidth(300.0f)
			]
		]
	];
}

TSharedRef<SWidget> SPhraseListItemWidget::GenerateCharacterOption(TSharedPtr<FString> Option)
{
	return SNew(STextBlock).Text(FText::FromString(*Option));
}

TSharedRef<SWidget> SPhraseListItemWidget::GenerateVarOption(TSharedPtr<FString> Option)
{
	return SNew(STextBlock).Text(FText::FromString(*Option));
}

TSharedRef<SWidget> SPhraseListItemWidget::GenerateComparisonOption(TSharedPtr<FString> Option)
{
	return SNew(STextBlock).Text(FText::FromString(*Option));
}

void SPhraseListItemWidget::OnCharacterSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	// Обработка выбора элемента из списка Character
}

void SPhraseListItemWidget::OnVarSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	// Обработка выбора элемента из списка Var
}

void SPhraseListItemWidget::OnComparisonSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	// из списка Comparison
}

void SPhraseListItemWidget::OnOrCheckStateChanged(ECheckBoxState NewState)
{
	// Обработка изменения состояния чекбокса "Or?"
}

FReply SPhraseListItemWidget::OnSettingsButtonClicked()
{
	// Обработка нажатия на кнопку настроек
	return FReply::Handled();
}

FReply SPhraseListItemWidget::OnHandButtonClicked()
{
	// Обработка нажатия на кнопку с иконкой руки
	return FReply::Handled();
}

FReply SPhraseListItemWidget::OnDeleteButtonClicked()
{
	// Обработка нажатия на кнопку удаления
	return FReply::Handled();
}

void SPhraseListItemWidget::OnMultiLineTextChanged(const FText& InText)
{
	// Обработка изменения текста в поле ввода с автоматическим расширением вниз
}