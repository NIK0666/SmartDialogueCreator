// SVarComboBox.cpp


#include "SVarComboBox.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"

void SVarComboBox::Construct(const FArguments& InArgs)
{
	SmartDialogueEditor = InArgs._SmartDialogueEditor;
	OnVarSelected = InArgs._OnVarSelected;
	DefaultText = InArgs._DefaultText;

	VarOptions = SmartDialogueEditor.Get()->GetAllVariablesList(true);

	TSharedPtr<FString> InitialSelectedItem = VarOptions[0];
	for (const TSharedPtr<FString>& Option : VarOptions)
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
		.OptionsSource(&VarOptions)
		.OnGenerateWidget(this, &SVarComboBox::GenerateVarOption)
		.OnSelectionChanged(this, &SVarComboBox::OnVarSelectedHandler)
		.InitiallySelectedItem(InitialSelectedItem)
		[
			SAssignNew(TextBlock, STextBlock)
			.MinDesiredWidth(32.f)
			.ToolTipText(NSLOCTEXT("SVarComboBox", "VarComboBoxTooltip", "Variable"))
			.Text(this, &SVarComboBox::GetCurrentVarText)
		]
	];
}

TSharedRef<SWidget> SVarComboBox::GenerateVarOption(TSharedPtr<FString> Option)
{
	return SNew(STextBlock).Text(FText::FromString(*Option));
}

void SVarComboBox::OnVarSelectedHandler(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	TextBlock->SetText(FText::FromString(*NewSelection.Get()));
	OnVarSelected.ExecuteIfBound(NewSelection);
}

FText SVarComboBox::GetCurrentVarText() const
{
	return FText::FromString(*ComboBox->GetSelectedItem());
}

