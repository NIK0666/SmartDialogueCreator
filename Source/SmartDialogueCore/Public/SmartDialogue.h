// 

#pragma once

#include "CoreMinimal.h"
#include "SmartDialogueData.h"
#include "SmartDialogueEditor/Private/Toolkit/SBranchInfoWidget.h"
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


	UFUNCTION(BlueprintCallable, Category = "SmartDialogue")
	void AddNewBranch(FSmartDialogueBranch& NewBranch);

	FName GenerateBranchName() const;

	FSmartDialogueBranch* GetBranchPtr(const FName& Name);
	bool RenameBranch(FName OldName, FName NewName);

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

	UPROPERTY(BlueprintReadOnly, Category = "SmartDialogue", meta = (AllowPrivateAccess = "true"))
	FName LastBranchName;
};
