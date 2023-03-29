// 

#pragma once

#include "CoreMinimal.h"
#include "SmartDialogueData.generated.h"

UENUM(BlueprintType)
enum class ESmartDialogueOperation : uint8
{
	EO_Equals UMETA(DisplayName = "="),
	EO_Plus UMETA(DisplayName = "+"),
	EO_Minus UMETA(DisplayName = "-"),
	EO_Multiply UMETA(DisplayName = "*"),
	EO_Divide UMETA(DisplayName = "/"),
};

UENUM(BlueprintType)
enum class ESmartDialogueEqualOperation : uint8
{
	EEO_Equals UMETA(DisplayName = "=="),
	EEO_GreaterOrEquals UMETA(DisplayName = ">="),
	EEO_LessOrEquals UMETA(DisplayName = "<="),
	EEO_Greater UMETA(DisplayName = ">"),
	EEO_Less UMETA(DisplayName = "<"),
};

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Id;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;
};

USTRUCT(BlueprintType)
struct FCustomParameterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Desc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Key;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Value;
};

USTRUCT(BlueprintType)
struct FVariableData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Desc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Key;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Value;
};

USTRUCT(BlueprintType)
struct FIf
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Else;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESmartDialogueEqualOperation EqualOperation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Value;
};

USTRUCT(BlueprintType)
struct FSmartDialoguePhrases
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FString> CustomParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIf If;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NPC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Random;
};


USTRUCT(BlueprintType)
struct FSmartDialogueEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	FString Param;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	bool Post;
};


USTRUCT(BlueprintType)
struct FSmartDialogueVars
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vars")
	FString Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vars")
	int32 Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vars")
	ESmartDialogueOperation Operation;
};

USTRUCT(BlueprintType)
struct FSmartDialogueBranch
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	TArray<FSmartDialoguePhrases> Phrases;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	bool Choice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	bool Closed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	bool OrCond;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	FString If;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	bool Hidden;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	bool HideSelf;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	TArray<FString> Show;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	FString ChangeStarted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	FSmartDialogueEvent Event;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	FSmartDialogueVars Vars;
};