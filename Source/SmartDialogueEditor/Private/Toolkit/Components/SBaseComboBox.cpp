// SBaseComboBox.cpp

#include "SBaseComboBox.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"


void SBaseComboBox::Construct(const FArguments& InArgs)
{
	SmartDialogueEditor = InArgs._SmartDialogueEditor;
	OnItemSelected = InArgs._OnItemSelected;
	DefaultText = InArgs._DefaultText;

	UpdateOptions();
	const TSharedPtr<FString> InitialSelectedItem = SetItemValue(DefaultText);	

	ChildSlot
	[
		SAssignNew(ComboBox, SComboBox<TSharedPtr<FString>>)
		.ContentPadding(FMargin(2.0f))
		.OptionsSource(&Options)
		.OnGenerateWidget(this, &SBaseComboBox::GenerateOption)
		.OnSelectionChanged(this, &SBaseComboBox::OnItemSelectedHandler)
		.OnComboBoxOpening(this, &SBaseComboBox::OnComboBoxOpeningHandler)
		.InitiallySelectedItem(InitialSelectedItem)
		[
			SAssignNew(TextBlock, STextBlock)
			.MinDesiredWidth(32.f)
			.Text(this, &SBaseComboBox::GetCurrentItemText)
		]
	];
}


const TSharedPtr<FString>& SBaseComboBox::SetItemValue(const FString& NewStringValue)
{
	for (const TSharedPtr<FString>& Option : Options)
	{
		if (Option->Equals(NewStringValue))
		{
			return Option;
		}
	}

	return Options[0];
}

TSharedRef<SWidget> SBaseComboBox::GenerateOption(TSharedPtr<FString> Option)
{
	return SNew(STextBlock).Text(FText::FromString(*Option));
}

void SBaseComboBox::OnItemSelectedHandler(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	if (NewSelection.IsValid())
	{
		TextBlock->SetText(FText::FromString(*NewSelection.Get()));
		OnItemSelected.ExecuteIfBound(NewSelection);
	}
}

FText SBaseComboBox::GetCurrentItemText() const
{
	if (ComboBox.IsValid() && ComboBox->GetSelectedItem().IsValid())
	{
		return FText::FromString(*ComboBox->GetSelectedItem());
	}
	return FText::GetEmpty();
}

void SBaseComboBox::OnComboBoxOpeningHandler()
{
    if (ComboBox.IsValid())
    {
        FSlateApplication::Get().SetKeyboardFocus(ComboBox);

        UpdateOptions();
        ComboBox->RefreshOptions();
    }
}

void SBaseComboBox::UpdateOptions()
{
	TArray<TSharedPtr<FString>> NewOptions = GetOptionsList();

	// Удаляем неактуальные элементы
	for (int32 i = Options.Num() - 1; i >= 0; i--)
	{
		if (!NewOptions.Contains(Options[i]))
		{
			Options.RemoveAt(i);
		}
	}

	// Добавляем новые элементы
	for (const TSharedPtr<FString>& NewOption : NewOptions)
	{
		if (!Options.Contains(NewOption))
		{
			Options.Add(NewOption);
		}
	}

	// Проверяем, существует ли выбранный элемент в обновленном списке
	if (ComboBox)
	{
		TSharedPtr<FString> CurrentSelectedItem = ComboBox->GetSelectedItem();

		if (CurrentSelectedItem.IsValid())
		{
			if (!Options.Contains(CurrentSelectedItem))
			{
				ComboBox->SetSelectedItem(Options[0]);
			}
		}
		else if (Options.Num() > 0)
		{
			// Если текущий выбранный элемент недействителен, устанавливаем первый элемент списка
			ComboBox->SetSelectedItem(Options[0]);
		}
	}
}