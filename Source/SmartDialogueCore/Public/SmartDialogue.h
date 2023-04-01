// 

#pragma once

#include "CoreMinimal.h"
#include "SmartDialogueData.h"
#include "SmartDialogue.generated.h"

DECLARE_EVENT(USmartDialogue, FOnBranchesChanged);

UCLASS(BlueprintType, Blueprintable)
class SMARTDIALOGUECORE_API USmartDialogue : public UObject
{
	GENERATED_BODY()

public:
	// Геттер для массива Branches
	UFUNCTION(BlueprintCallable, Category = "SmartDialogue")
	TMap<FName, FSmartDialogueBranch> GetBranches() const;

	// Геттер для AutoBranch
	UFUNCTION(BlueprintCallable, Category = "SmartDialogue")
	FString GetAutoBranch() const;

	// Сеттер для AutoBranch
	UFUNCTION(BlueprintCallable, Category = "SmartDialogue")
	void SetAutoBranch(const FString& NewAutoBranch);

	// Геттер для Character
	UFUNCTION(BlueprintCallable, Category = "SmartDialogue")
	FString GetCharacter() const;

	// Сеттер для AutoBranch
	UFUNCTION(BlueprintCallable, Category = "SmartDialogue")
	void SetCharacter(const FString& NewCharacter);

	// Геттер для Variables
	UFUNCTION(BlueprintPure, Category = "Smart Dial")
	TArray<FVariableData> GetVariables() const;
	
	// Сеттер для Variables
	UFUNCTION(BlueprintCallable, Category = "Smart Dial")
	void SetVariables(const TArray<FVariableData>& NewVariables);


	UFUNCTION(BlueprintCallable, Category = "SmartDialogue")
	void AddNewBranch(FSmartDialogueBranch& NewBranch);
	
	UFUNCTION(BlueprintCallable, Category = "SmartDialogue")
	void AddNewVariable(FVariableData& NewVariable);


	FName GenerateBranchName() const;

	FSmartDialogueBranch* GetBranchPtr(const FName& Name);
	bool RenameBranch(FName OldName, FName NewName);		
	bool RemoveBranch(FName BranchName);
	bool RemoveShowBranch(FName BranchName, const FString& RemoveString);
	
	bool RemoveVarOperation(FName BranchName, const int32 Index);
	bool RemoveIfOperation(FName BranchName, int32 Index);
	void AddVarOperation(FName BranchName, const FString& VarName, const FString& OperationString, int32 Value);
	void AddIfOperation(const FName& BranchName, const FString& VarName, const FString& OperationString, int32 Value);
	
	bool RemoveHideBranch(FName BranchName, const FString& RemoveString);
	void AddShowBranch(const FName& BranchName, const FString& AddString);
	void AddHideBranch(const FName& BranchName, const FString& AddString);

	FOnBranchesChanged OnBranchesChanged;

protected:
	UFUNCTION(BlueprintCallable, Category = "SmartDialogue")
	void BranchesChanged();

	void MarkAsDirty();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SmartDialogue", meta = (AllowPrivateAccess = "true"))
	TMap<FName, FSmartDialogueBranch> Branches;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SmartDialogue", meta = (AllowPrivateAccess = "true"))
	FString AutoBranch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SmartDialogue", meta = (AllowPrivateAccess = "true"))
	FString Character;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SmartDialogue", meta = (AllowPrivateAccess = "true"))
	TArray<FVariableData> Variables;
	
	UPROPERTY(BlueprintReadOnly, Category = "SmartDialogue", meta = (AllowPrivateAccess = "true"))
	FName LastBranchName;
};
