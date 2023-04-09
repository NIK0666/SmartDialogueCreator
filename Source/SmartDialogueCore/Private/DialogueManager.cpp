// DialogueManager.cpp
#include "DialogueManager.h"

#include "SmartDialConfig.h"
#include "SmartDialogue.h"

void UDialogueManager::InitializeDialogueProgress(USmartDialConfig* InDialConfig)
{
	// Initialize the dialogue progress with InDialConfig
	// Fill PublicVars
	this->DialConfig = InDialConfig;
	DialogueProgress = {};
	for (auto Element : DialConfig->GetVariables())
	{
		DialogueProgress.PublicVars.Add(Element.Key, Element.Value);
	}
}

void UDialogueManager::StartDialogue(USmartDialogue* DialogueAsset)
{
	// Implement the dialogue starting logic
	// Steps 1-4 from your algorithm
	CurrentDialogue = DialogueAsset;
	UpdateDialogueProgress(DialogueAsset);

	FDialogueProgress& DialogueProg = DialogueProgress.Dials.FindOrAdd(DialogueAsset->GetDialogueId());
	OnHideBranchOptions.Broadcast();
	if (!DialogueProg.Auto.IsEmpty())
	{
		PlayBranch(FName(*DialogueProg.Auto)); 
		DialogueProg.Auto = "";
	}
	else
	{
		ShowBranchOptions();
	}
}

void UDialogueManager::ShowNextPhrase()
{
	if (CurrentPhraseIndex >= CurrentBranch.Phrases.Num())
	{
		return; // Exit if there are no more phrases in the current branch
	}
	// Implement the logic to show the next phrase
	// Steps 6 and 7 from your algorithm
	CurrentPhraseIndex++;
	if (CurrentPhraseIndex < CurrentBranch.Phrases.Num())
	{
		FSmartDialoguePhrase& CurrentPhrase = CurrentBranch.Phrases[CurrentPhraseIndex];
		OnShowPhrase.Broadcast(CurrentPhrase.Text, CurrentPhrase.NPC);
	}
	else
	{
		// 7.1 Check HideSelf
		if (CurrentBranch.HideSelf)
		{
			DialogueProgress.Dials[CurrentDialogue->GetDialogueId()].Hidden.Add(CurrentBranch.Name.ToString());
		}

		// 7.2 Trigger event if needed
		if (CurrentBranch.Event.Post)
		{
			TriggerEventIfValid(CurrentBranch.Event);
		}

		// 7.3 Close the dialogue if needed
		if (CurrentBranch.Closed)
		{
			OnCloseDialogue.Broadcast();
		}
		// Check for Choice
		else if (CurrentBranch.Choice)
		{
			ShowChoiceOptions(CurrentBranch.Show);
		}
		else
		{
			// 7.4 Show branch options again
			ShowBranchOptions();
		}
	}
}

void UDialogueManager::SelectBranch(int32 BranchIndex)
{
	// Implement the logic to select and play a branch
	FName SelectedBranchName;
	int32 i = 0;
	for (const auto& Branch : CurrentDialogue->GetBranches())
	{
		if (i == CurrentBranchIndices[BranchIndex])
		{
			SelectedBranchName = Branch.Key;
			break;
		}
		i++;
	}
	OnHideBranchOptions.Broadcast();
	PlayBranch(SelectedBranchName);
}


void UDialogueManager::UpdateDialogueProgress(USmartDialogue* DialogueAsset)
{
	// Implement the logic to update the dialogue progress
	// Steps 2.1, 2.1.1, and 2.1.2 from your algorithm
	FDialogueProgress& DialogueProg = DialogueProgress.Dials.FindOrAdd(DialogueAsset->GetDialogueId());
	
	if (!DialogueAsset->GetAutoBranch().IsEmpty())
	{		
		DialogueProg.Auto = DialogueAsset->GetAutoBranch();
	}
	for (auto Element : DialogueAsset->GetBranches())
	{
		if (Element.Value.Hidden)
		{
			DialogueProg.Hidden.Add(Element.Key.ToString());
		}
		
	}

	for (const FVariableData& VarData : DialogueAsset->GetVariables())
	{
		FCharactersVarsProgress& CharVarProg = DialogueProgress.CharVars.FindOrAdd(DialogueAsset->GetCharacter());
		CharVarProg.Variable.FindOrAdd(VarData.Key) = VarData.Value;
	}
}
void UDialogueManager::PlayBranch(const FName& BranchName)
{
	// Implement the logic to play a dialogue branch
	// Steps 5-7 from your algorithm
	CurrentBranch = CurrentDialogue->GetBranches()[BranchName];

	if (!CurrentBranch.Event.Post)
	{
		TriggerEventIfValid(CurrentBranch.Event);
	}

	
	if  (!CurrentBranch.Choice && CurrentBranch.Show.Num() > 0)
	{
		FDialogueProgress& DialogueProg = DialogueProgress.Dials.FindOrAdd(CurrentDialogue->GetDialogueId());
		for (auto L_ShowBranch : CurrentBranch.Show)
		{
			DialogueProg.Hidden.Remove(L_ShowBranch);
		}
	}

	ProcessBranchVars(CurrentBranch.Vars);

	if (CurrentBranch.Phrases.Num() > 0)
	{
		// Implement the logic to play a dialogue branch
		// Steps 5-7 from your algorithm
		CurrentPhraseIndex = 0;
		FSmartDialoguePhrase& CurrentPhrase = CurrentBranch.Phrases[CurrentPhraseIndex];
		OnShowPhrase.Broadcast(CurrentPhrase.Text, CurrentPhrase.NPC);
	}
	else
	{
		ShowNextPhrase(); // If no phrases in branch, move to the next phrase or end the branch
	}
}

bool UDialogueManager::ValidConditions(TArray<FIf> IfConditions)
{
	return true;
}

void UDialogueManager::ShowBranchOptions()
{
	
	// Implement the logic to show branch options
	// Step 4 from your algorithm
	CurrentBranchTexts.Empty();
	CurrentBranchIndices.Empty();
	auto Hidden = DialogueProgress.Dials[CurrentDialogue->GetDialogueId()].Hidden;
	int32 i = 0;
	for (auto Element : CurrentDialogue->GetBranches())
	{
		if (!Hidden.Contains(Element.Key.ToString()) && ValidConditions(Element.Value.If))
		{
			CurrentBranchTexts.Add(Element.Value.Text);
			CurrentBranchIndices.Add(i);
		}
		i++;
	}
	if (CurrentBranchTexts.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("No valid options found!"));
		OnCloseDialogue.Broadcast();
	}
	else
	{
		OnShowBranchOptions.Broadcast(CurrentBranchTexts, CurrentBranchIndices);
	}
}

void UDialogueManager::ShowChoiceOptions(const TArray<FString>& ChoicesToShow)
{
	CurrentBranchTexts.Empty();
	CurrentBranchIndices.Empty();
	auto Branches = CurrentDialogue->GetBranches();

	for (const FString& ChoiceName : ChoicesToShow)
	{
		FName ChoiceFName(ChoiceName);
		if (Branches.Contains(ChoiceFName) && ValidConditions(Branches[ChoiceFName].If))
		{
			CurrentBranchTexts.Add(Branches[ChoiceFName].Text);
			int32 BranchIndex = 0;
			for (const auto& Branch : Branches)
			{
				if (Branch.Key == ChoiceFName)
				{
					CurrentBranchIndices.Add(BranchIndex);
					break;
				}
				BranchIndex++;
			}
		}
	}

	// If there is only one option, automatically play it without broadcasting the delegate
	if (CurrentBranchIndices.Num() == 1)
	{
		SelectBranch(0);
	}
	// If there are no options, log an error and call ShowBranchOptions()
	else if (CurrentBranchIndices.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No valid choice options found!"));
		ShowBranchOptions();
	}
	// Otherwise, broadcast the delegate OnShowBranchOptions
	else
	{
		OnShowBranchOptions.Broadcast(CurrentBranchTexts, CurrentBranchIndices);
	}
}

void UDialogueManager::ProcessBranchVars(const TArray<FSmartDialogueVars>& Vars)
{
	for (const FSmartDialogueVars& Var : Vars)
	{
		int32* PublicVarPtr = DialogueProgress.PublicVars.Find(Var.Key);
		if (PublicVarPtr)
		{
			int32 OldValue = *PublicVarPtr;
			*PublicVarPtr = PerformOperation(*PublicVarPtr, Var.Value, Var.Operation);
			OnVariableChanged.Broadcast(true, Var.Key, OldValue, *PublicVarPtr);
		}
		else
		{
			FCharactersVarsProgress* CharVarPtr = DialogueProgress.CharVars.Find(CurrentDialogue->GetCharacter());
			if (CharVarPtr)
			{
				int32* ValuePtr = CharVarPtr->Variable.Find(Var.Key);
				if (ValuePtr)
				{
					int32 OldValue = *ValuePtr;
					*ValuePtr = PerformOperation(*ValuePtr, Var.Value, Var.Operation);
					OnVariableChanged.Broadcast(false, Var.Key, OldValue, *ValuePtr);
				}
				else
				{
					CharVarPtr->Variable.Add(Var.Key, 0);
					int32 OldValue = CharVarPtr->Variable[Var.Key];
					CharVarPtr->Variable[Var.Key] = PerformOperation(CharVarPtr->Variable[Var.Key], Var.Value, Var.Operation);
					OnVariableChanged.Broadcast(false, Var.Key, OldValue, CharVarPtr->Variable[Var.Key]);
				}
			}
			else
			{
				DialogueProgress.PublicVars.Add(Var.Key, 0);
				int32 OldValue = DialogueProgress.PublicVars[Var.Key];
				DialogueProgress.PublicVars[Var.Key] = PerformOperation(DialogueProgress.PublicVars[Var.Key], Var.Value, Var.Operation);
				OnVariableChanged.Broadcast(true, Var.Key, OldValue, DialogueProgress.PublicVars[Var.Key]);
			}
		}
	}
}


int32 UDialogueManager::PerformOperation(int32 TargetValue, int32 OperandValue, ESmartDialogueOperation Operation)
{
	int32 ResultValue = TargetValue;

	switch (Operation)
	{
	case ESmartDialogueOperation::EO_Equals:
		ResultValue = OperandValue;
		break;
	case ESmartDialogueOperation::EO_Plus:
		ResultValue += OperandValue;
		break;
	case ESmartDialogueOperation::EO_Minus:
		ResultValue -= OperandValue;
		break;
	case ESmartDialogueOperation::EO_Multiply:
		ResultValue *= OperandValue;
		break;
	case ESmartDialogueOperation::EO_Divide:
		if (OperandValue != 0)
		{
			ResultValue /= OperandValue;
		}
		break;
	}

	return ResultValue;
}


void UDialogueManager::TriggerEventIfValid(const FSmartDialogueEvent& Event)
{
	// Implement the logic to trigger event if valid
	if (!Event.Name.IsEmpty())
	{
		OnEventTriggered.Broadcast(Event.Name, Event.Param);
	}
}