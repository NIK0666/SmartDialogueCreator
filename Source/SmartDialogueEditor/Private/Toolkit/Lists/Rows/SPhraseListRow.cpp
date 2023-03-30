// SPhraseListRow.cpp


#include "SPhraseListRow.h"

#include "EditorStyleSet.h"
#include "Toolkit/FSmartDialogueEditor.h"
#include "Toolkit/Components/SCharacterComboBox.h"
#include "Toolkit/Components/SVarComboBox.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"


void SPhraseListRow::Construct(const FArguments& InArgs)
{
	SmartDialogueEditor = InArgs._SmartDialogueEditor;
	SmartDialoguePhrasePtr = InArgs._SmartDialoguePhrasePtr;
	
	CharacterOptions = SmartDialogueEditor.Get()->GetAllCharactersList(true);
	VarOptions = SmartDialogueEditor.Get()->GetAllVariablesList(true);

	ComparisonOptions = SmartDialogueEditor.Get()->GetOperations(false);
	
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
				SAssignNew(CharacterComboBox, SCharacterComboBox)
				.SmartDialogueEditor(SmartDialogueEditor)
				.OnCharacterSelected(this, &SPhraseListRow::OnCharacterSelected)
				.DefaultText(SmartDialoguePhrasePtr->NPC)
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
				SAssignNew(VarComboBox, SVarComboBox)
				.SmartDialogueEditor(SmartDialogueEditor)
				.OnVarSelected(this, &SPhraseListRow::OnVarSelected)
				.DefaultText(SmartDialoguePhrasePtr->If.Key)
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
					SAssignNew(ComparisonTextBlock, STextBlock)
					.Text(this, &SPhraseListRow::GetCurrentComparisonText)
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

void SPhraseListRow::OnCharacterSelected(TSharedPtr<FString> NewSelection)
{
    SmartDialoguePhrasePtr->NPC = *NewSelection;
}

void SPhraseListRow::OnVarSelected(TSharedPtr<FString> NewSelection)
{
    SmartDialoguePhrasePtr->If.Key = *NewSelection;
}

void SPhraseListRow::OnComparisonSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
    ComparisonTextBlock->SetText(FText::FromString(*NewSelection));
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

FText SPhraseListRow::GetCurrentCharacterText() const
{
	return FText::FromString(SmartDialoguePhrasePtr->NPC);
}

FText SPhraseListRow::GetCurrentVarText() const
{
	return FText::FromString(SmartDialoguePhrasePtr->If.Key);
}

FText SPhraseListRow::GetCurrentComparisonText() const
{
	return FText::FromString(*ComparisonComboBox->GetSelectedItem());
}