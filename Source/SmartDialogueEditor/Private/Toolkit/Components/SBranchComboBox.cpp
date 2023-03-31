// 


#include "SBranchComboBox.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"

void SBranchComboBox::Construct(const FArguments& InArgs)
{
	SBaseComboBox::Construct(SBaseComboBox::FArguments()
		.SmartDialogueEditor(InArgs._SmartDialogueEditor)
		.OnItemSelected(InArgs._OnItemSelected)
		.DefaultText(InArgs._DefaultText));
	TextBlock->SetToolTipText(NSLOCTEXT("SBranchComboBox", "BranchComboBoxTooltip", "Branch"));
}

TArray<TSharedPtr<FString>> SBranchComboBox::GetOptionsList()
{
	return SmartDialogueEditor->GetAllBranchesList();
}