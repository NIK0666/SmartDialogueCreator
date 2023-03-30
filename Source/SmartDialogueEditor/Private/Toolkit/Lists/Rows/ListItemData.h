// 

#pragma once

#include "CoreMinimal.h"
#include "ListItemData.generated.h"

USTRUCT(BlueprintType)
struct FListItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString Name;

	bool operator==(const FListItemData& Other) const
	{
		return Name == Other.Name;
	}

	bool operator!=(const FListItemData& Other) const
	{
		return !(*this == Other);
	}
};

USTRUCT(BlueprintType)
struct FOperationItemData : public FListItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	ESmartDialogueEqualOperation EqualOperation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Value;
	
};