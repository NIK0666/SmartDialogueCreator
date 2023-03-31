// SVarComboBox.cpp


#include "SVarComboBox.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"

// Заменить существующий метод Construct на следующий:
void SVarComboBox::Construct(const FArguments& InArgs)
{
	SBaseComboBox::Construct(SBaseComboBox::FArguments()
		.SmartDialogueEditor(InArgs._SmartDialogueEditor)
		.OnItemSelected(InArgs._OnItemSelected)
		.DefaultText(InArgs._DefaultText));
	TextBlock->SetToolTipText(NSLOCTEXT("SVarComboBox", "VarComboBoxTooltip", "Variable"));
}
TArray<TSharedPtr<FString>> SVarComboBox::GetOptionsList()
{
	return SmartDialogueEditor.Get()->GetAllVariablesList(true);
}
