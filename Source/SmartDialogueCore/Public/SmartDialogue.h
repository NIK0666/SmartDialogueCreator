// 

#pragma once

#include "CoreMinimal.h"
#include "SmartDialogueData.h"
#include "SmartDialogue.generated.h"

UCLASS(Blueprintable)
class SMARTDIALOGUECORE_API USmartDialogue : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smart Dialogue")
	FString AutoBranch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Smart Dialogue")
	TArray<FSmartDialogueBranch> Branches;
};
