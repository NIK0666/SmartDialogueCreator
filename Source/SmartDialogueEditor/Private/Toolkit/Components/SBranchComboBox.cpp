// 


#include "SBranchComboBox.h"

#include "SmartDialogue.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"


void SBranchComboBox::Construct(const FArguments& InArgs)
{
	SBaseComboBox::Construct(SBaseComboBox::FArguments()
		.SmartDialogueEditor(InArgs._SmartDialogueEditor)
		.OnItemSelected(InArgs._OnItemSelected)
		.DefaultText(InArgs._DefaultText));
	TextBlock->SetToolTipText(NSLOCTEXT("SBranchComboBox", "BranchComboBoxTooltip", "Branch"));

	SmartDialogueEditor->GetDialogue()->OnBranchRenamed.AddSP(this, &SBranchComboBox::OnRenamedBranch);

}

TArray<TSharedPtr<FString>> SBranchComboBox::GetOptionsList()
{
	return SmartDialogueEditor->GetAllBranchesList();
}

void SBranchComboBox::OnRenamedBranch(FName OldName, FName NewName)
{
	if (TextBlock->GetText().ToString() == OldName.ToString())
	{
		UpdateOptions();
		const TSharedPtr<FString> InitialSelectedItem = SetItemValue(NewName.ToString());	
		TextBlock->SetText(FText::FromString(*InitialSelectedItem));
	}
}