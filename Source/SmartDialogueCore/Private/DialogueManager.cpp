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
	bDialogueProgressInitialized = true;
}

void UDialogueManager::StartDialogue(USmartDialogue* DialogueAsset)
{
	// Implement the dialogue starting logic
	// Steps 1-4 from your algorithm
	CurrentDialogue = DialogueAsset;
	if (!DialogueProgress.Dials.Find(DialogueAsset->GetDialogueId()))
	{
		UpdateDialogueProgress(DialogueAsset);
	}
	
	bNeedDialogueClose = false;
	
	FDialogueProgress& DialogueProg = DialogueProgress.Dials.FindOrAdd(DialogueAsset->GetDialogueId());
	OnHideBranchOptions.Broadcast();
	if (!DialogueProg.Auto.IsEmpty())
	{
		const FName AutoBranch = FName(*DialogueProg.Auto);
		DialogueProg.Auto = "";
		PlayBranch(AutoBranch); 
	}
	else
	{
		ShowBranchOptions();
	}
}

void UDialogueManager::ShowNextPhrase() {
	if (bNeedDialogueClose)
	{		
		OnCloseDialogue.Broadcast();
		bNeedDialogueClose = false;
		return;
	}
	
	bCanShowOneChoice = true;
	
	if (CurrentBranch.Queue) {
		FDialogueProgress& DialogueProg = DialogueProgress.Dials.FindOrAdd(CurrentDialogue->GetDialogueId());
		int32& QueueIndex = DialogueProg.Queue.FindOrAdd(CurrentBranch.Name);

		if (QueueIndex >= CurrentBranch.Phrases.Num()) {
			QueueIndex = 0;
		}

		CurrentPhraseIndex = QueueIndex;
		QueueIndex++;

		// Обновляем значение индекса очереди в DialogueProg.Queue
		DialogueProg.Queue[CurrentBranch.Name] = QueueIndex;

		const auto Condition = CurrentBranch.Phrases[CurrentPhraseIndex].If;
		if (!Condition.Key.IsEmpty()) {
			const bool bIsValid = ValidCondition(Condition);
			if (!bIsValid) {
				ShowNextPhrase();
				return;
			}
		}

		FSmartDialoguePhrase& CurrentPhrase = CurrentBranch.Phrases[CurrentPhraseIndex];
		OnShowPhrase.Broadcast(CurrentPhrase.Text, CurrentPhrase.NPC);
		PerformPostPhraseActions();
		return;
	}
	
    CurrentPhraseIndex++;
	
    if (CurrentPhraseIndex < CurrentBranch.Phrases.Num()) {
        const auto Condition = CurrentBranch.Phrases[CurrentPhraseIndex].If;
        if (!Condition.Key.IsEmpty()) {
            const bool bIsValid = ValidCondition(Condition);
            if (!bIsValid) {
                ShowNextPhrase();
                return;
            }
        }

        FSmartDialoguePhrase& CurrentPhrase = CurrentBranch.Phrases[CurrentPhraseIndex];
        OnShowPhrase.Broadcast(CurrentPhrase.Text, CurrentPhrase.NPC);
    	bCanShowOneChoice = false;
    }
    
    if (CurrentPhraseIndex >= CurrentBranch.Phrases.Num() || !HasValidRemainingPhrases()) {
        PerformPostPhraseActions();
    }
}

void UDialogueManager::PerformPostPhraseActions() {
	// 7.1 Check HideSelf
	if (CurrentBranch.HideSelf) {
		DialogueProgress.Dials[CurrentDialogue->GetDialogueId()].Hidden.Add(CurrentBranch.Name.ToString());
	}

	// 7.2 Trigger event if needed
	if (CurrentBranch.Event.Post) {
		TriggerEventIfValid(CurrentBranch.Event);
	}

	// 7.3 Close the dialogue if needed
	if (CurrentBranch.Closed) {
		bNeedDialogueClose = true;
	}
	// Check for Choice
	else if (CurrentBranch.Choice) {
		ShowChoiceOptions(CurrentBranch.Show);
	} else {
		// 7.4 Show branch options again
		ShowBranchOptions();
	}
}

bool UDialogueManager::HasValidRemainingPhrases() {
	for (int32 i = CurrentPhraseIndex + 1; i < CurrentBranch.Phrases.Num(); ++i) {
		const auto Condition = CurrentBranch.Phrases[i].If;
		if (Condition.Key.IsEmpty() || ValidCondition(Condition)) {
			return true;
		}
	}

	return false;
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
			DialogueProg.Hidden.AddUnique(Element.Key.ToString());
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
	
	if (!CurrentBranch.ChangeStarted.IsEmpty())
	{
		if (auto DialogueProg = DialogueProgress.Dials.Find(CurrentDialogue->GetDialogueId()))
		{
			DialogueProg->Auto = CurrentBranch.ChangeStarted;
		}
	}
	
	ProcessBranchVars(CurrentBranch.Vars);

	CurrentPhraseIndex = -1;
	ShowNextPhrase();
}

bool UDialogueManager::ValidConditions(TArray<FIf> Conditions)
{
	for (auto Element : Conditions)
	{
		if (!ValidCondition(Element))
		{
			return false;
		}
	}
	
	return true;
}

bool UDialogueManager::ValidCondition(FIf Condition)
{
	int32* VarValuePtr = nullptr;

	// Проверяем, есть ли переменная в PublicVars
	VarValuePtr = DialogueProgress.PublicVars.Find(Condition.Key);

	// Если нет, ищем в CharVars для текущего персонажа
	if (!VarValuePtr)
	{
		FCharactersVarsProgress* CharVarPtr = DialogueProgress.CharVars.Find(CurrentDialogue->GetCharacter());
		if (CharVarPtr)
		{
			VarValuePtr = CharVarPtr->Variable.Find(Condition.Key);
		}
	}

	// Если переменная не найдена, условие считается недействительным
	if (!VarValuePtr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Condition variable not found: %s"), *Condition.Key);
		return false;
	}
	const int32 Value = *VarValuePtr;

	// Сравниваем значение переменной с указанным значением в условии
	switch (Condition.EqualOperation)
	{
	case ESmartDialogueEqualOperation::EEO_Equals:
		UE_LOG(LogTemp, Log, TEXT("Condition: %s equals %d"), *Condition.Key, Condition.Value);
		return Value == Condition.Value;
	case ESmartDialogueEqualOperation::EEO_GreaterOrEquals:
		UE_LOG(LogTemp, Log, TEXT("Condition: %s greater or equal to %d"), *Condition.Key, Condition.Value);
		return Value >= Condition.Value;
	case ESmartDialogueEqualOperation::EEO_LessOrEquals:
		UE_LOG(LogTemp, Log, TEXT("Condition: %s less or equal to %d"), *Condition.Key, Condition.Value);
		return Value <= Condition.Value;
	case ESmartDialogueEqualOperation::EEO_Greater:
		UE_LOG(LogTemp, Log, TEXT("Condition: %s greater than %d"), *Condition.Key, Condition.Value);
		return Value > Condition.Value;
	case ESmartDialogueEqualOperation::EEO_Less:
		UE_LOG(LogTemp, Log, TEXT("Condition: %s less than %d"), *Condition.Key, Condition.Value);
		return Value < Condition.Value;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid condition operation for variable: %s"), *Condition.Key);
		return false;
	}
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
		if (bCanShowOneChoice)
		{
			SelectBranch(0);
		}		
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