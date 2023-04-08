// DialoguePlayerData.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialoguePlayerData.generated.h"


USTRUCT(BlueprintType)
struct FDialogueElementData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Content;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor TitleColor = FLinearColor::Gray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor ContentColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsTitleVisible = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsContentVisible = true;
};