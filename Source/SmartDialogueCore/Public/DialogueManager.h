// DialogueManager.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SmartDialogueData.h"

#include "DialogueManager.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnShowBranchOptionsDelegate, const TArray<FText>& /*BranchTexts*/, const TArray<int32>& /*BranchIndices*/);
DECLARE_MULTICAST_DELEGATE(FOnHideBranchOptionsDelegate);

using FCustomParamsMap = TMap<FString, FString>;
DECLARE_MULTICAST_DELEGATE_FourParams(FOnShowPhraseDelegate, const FText& /*PhraseText*/, const FString& /*SpeakerId*/,
	const FString& /*AnimName*/, const FCustomParamsMap& /*CustomParams*/);

DECLARE_MULTICAST_DELEGATE(FOnCloseDialogueDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEventTriggeredDelegate, const FString& /*EventName*/, const FString& /*EventParam*/);
DECLARE_MULTICAST_DELEGATE_FourParams(FOnVariableChangedDelegate, bool /*bIsPublic*/, const FString& /*VarName*/, int32 /*OldValue*/, int32 /*NewValue*/);


class USmartDialConfig;
class USmartDialogue;

UCLASS(Blueprintable)
class SMARTDIALOGUECORE_API UDialogueManager : public UObject
{
	GENERATED_BODY()

public:
	// Methods for initialization and starting the dialogue
	void InitializeDialogueProgress(USmartDialConfig* InDialConfig);
	void StartDialogue(USmartDialogue* DialogueAsset, const FString& InBranch = "");

	// Methods to control dialogue playback
	UFUNCTION(BlueprintCallable)
	void ShowNextPhrase();
	
	void PerformPostPhraseActions();
	bool HasValidRemainingPhrases();
	void SelectBranch(int32 BranchIndex);
	FInfoProgress GetInfoProgress() const { return DialogueProgress; }
	void SetInfoProgress(const FInfoProgress& InfoProgress) { DialogueProgress = InfoProgress; }

	UFUNCTION(BlueprintPure)
	bool IsActiveDialog();
	
	bool IsDialogueProgressInitialized() { return bDialogueProgressInitialized; }

	// Delegates for handling dialogue events
	FOnShowBranchOptionsDelegate OnShowBranchOptions;
	FOnHideBranchOptionsDelegate OnHideBranchOptions;
	FOnShowPhraseDelegate OnShowPhrase; 
	FOnCloseDialogueDelegate OnCloseDialogue;
	FOnEventTriggeredDelegate OnEventTriggered;
	FOnVariableChangedDelegate OnVariableChanged;

	
	bool bNeedDialogueClose = false;
	bool bIsActiveDialog = false;


private:
	// Helper methods for dialogue processing
	void UpdateDialogueProgress(USmartDialogue* DialogueAsset);
	void PlayBranch(const FName& BranchName);
	bool ValidConditions(TArray<FIf> Conditions);
	bool ValidCondition(FIf Condition);
	void ShowBranchOptions();
	void ShowChoiceOptions(const TArray<FString>& ChoicesToShow);
	void ProcessBranchVars(const TArray<FSmartDialogueVars>& Vars);
	int32 PerformOperation(int32 TargetValue, int32 OperandValue, ESmartDialogueOperation Operation);
	void TriggerEventIfValid(const FSmartDialogueEvent& Event);

	// Dialogue data and progress
	USmartDialogue* CurrentDialogue;
	USmartDialConfig* DialConfig;
	FInfoProgress DialogueProgress;
	TArray<FText> CurrentBranchTexts;
	TArray<int32> CurrentBranchIndices;
	int32 CurrentPhraseIndex;
	FSmartDialogueBranch CurrentBranch;
	bool bDialogueProgressInitialized = false;
	bool bCanShowOneChoice = true;
};