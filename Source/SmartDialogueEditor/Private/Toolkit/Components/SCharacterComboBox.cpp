// SCharacterComboBox.cpp
#include "SCharacterComboBox.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"

void SCharacterComboBox::Construct(const FArguments& InArgs)
{
	SmartDialogueEditor = InArgs._SmartDialogueEditor;
	OnCharacterSelected = InArgs._OnCharacterSelected;
	DefaultText = InArgs._DefaultText;

	CharacterOptions = SmartDialogueEditor.Get()->GetAllCharactersList(true);

	// Find the item with the specified default text
	TSharedPtr<FString> InitialSelectedItem = CharacterOptions[0];
	for (const TSharedPtr<FString>& Option : CharacterOptions)
	{
		if (Option->Equals(DefaultText))
		{
			InitialSelectedItem = Option;
			break;
		}
	}
	
	ChildSlot
	[
		SAssignNew(ComboBox, SComboBox<TSharedPtr<FString>>)
		.ContentPadding(FMargin(2.0f))
		.OptionsSource(&CharacterOptions)
		.OnGenerateWidget(this, &SCharacterComboBox::GenerateCharacterOption)
		.OnSelectionChanged(this, &SCharacterComboBox::OnCharacterSelectedHandler)
		.OnComboBoxOpening(this, &SCharacterComboBox::OnComboBoxOpeningHandler)
		.InitiallySelectedItem(InitialSelectedItem) // Use the found item
		[
			SAssignNew(TextBlock, STextBlock)
			.MinDesiredWidth(32.f)
			.ToolTipText(NSLOCTEXT("SCharacterComboBox", "CharacterComboBoxTooltip", "Character"))
			.Text(this, &SCharacterComboBox::GetCurrentCharacterText)
		]
	];
}

TSharedRef<SWidget> SCharacterComboBox::GenerateCharacterOption(TSharedPtr<FString> Option)
{	
	return SNew(STextBlock).Text(FText::FromString(*Option));
}

void SCharacterComboBox::OnCharacterSelectedHandler(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	TextBlock->SetText(FText::FromString(*NewSelection.Get()));
	OnCharacterSelected.ExecuteIfBound(NewSelection);
}

FText SCharacterComboBox::GetCurrentCharacterText() const
{
	if (ComboBox.IsValid() && ComboBox->GetSelectedItem().IsValid())
	{
		return FText::FromString(*ComboBox->GetSelectedItem());
	}
	return FText::GetEmpty();
}

void SCharacterComboBox::OnComboBoxOpeningHandler()
{
	if (ComboBox.IsValid())
	{
		// Set the focus on the ComboBox before it opens
		FSlateApplication::Get().SetKeyboardFocus(ComboBox);
	}
}