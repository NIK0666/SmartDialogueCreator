// SCharacterComboBox.cpp
#include "SCharacterComboBox.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"

// Заменить существующий метод Construct на следующий:
void SCharacterComboBox::Construct(const FArguments& InArgs)
{
	SBaseComboBox::Construct(SBaseComboBox::FArguments()
		.SmartDialogueEditor(InArgs._SmartDialogueEditor)
		.OnItemSelected(InArgs._OnItemSelected)
		.DefaultText(InArgs._DefaultText));
	TextBlock->SetToolTipText(NSLOCTEXT("SCharacterComboBox", "CharacterComboBoxTooltip", "Character"));
}

// Добавьте новый метод UpdateOptions в SCharacterComboBox:
TArray<TSharedPtr<FString>> SCharacterComboBox::GetOptionsList()
{
	return  SmartDialogueEditor.Get()->GetAllCharactersList(true);
}