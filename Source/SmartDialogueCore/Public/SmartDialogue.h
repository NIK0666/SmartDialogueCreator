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
	TArray<FSmartDialogueBranch> GetBranches() const;

	// Сеттер для массива Branches
	UFUNCTION(BlueprintCallable, Category = "SmartDialogue")
	void SetBranches(const TArray<FSmartDialogueBranch>& NewBranches);

	// Геттер для AutoBranch
	UFUNCTION(BlueprintCallable, Category = "SmartDialogue")
	FString GetAutoBranch() const;

	// Сеттер для AutoBranch
	UFUNCTION(BlueprintCallable, Category = "SmartDialogue")
	void SetAutoBranch(const FString& NewAutoBranch);


	UFUNCTION(BlueprintCallable, Category = "SmartDialogue")
	void AddNewBranch(FSmartDialogueBranch& NewBranch)
	{
		Branches.Add(NewBranch);
		BranchesChanged();
	}

	FOnBranchesChanged OnBranchesChanged;

protected:
	UFUNCTION(BlueprintCallable, Category = "SmartDialogue")
	void BranchesChanged();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SmartDialogue", meta = (AllowPrivateAccess = "true"))
	TArray<FSmartDialogueBranch> Branches;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SmartDialogue", meta = (AllowPrivateAccess = "true"))
	FString AutoBranch;
};
