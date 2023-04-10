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

class ESmartDialogueEqualOperationHelper
{
public:
	static FString EnumOperationToString(ESmartDialogueEqualOperation Operation)
	{
		switch (Operation)
		{
		case ESmartDialogueEqualOperation::EEO_Equals:
			return "==";
		case ESmartDialogueEqualOperation::EEO_Greater:
			return ">";
		case ESmartDialogueEqualOperation::EEO_Less:
			return "<";
		case ESmartDialogueEqualOperation::EEO_GreaterOrEquals:
			return ">=";
		case ESmartDialogueEqualOperation::EEO_LessOrEquals:
			return "<=";
		}

		return "";
	}
	static ESmartDialogueEqualOperation EnumOperationFromString(const FString& OperationString)
	{
		if (OperationString == "==")
		{
			return ESmartDialogueEqualOperation::EEO_Equals;
		}
		if (OperationString == ">")
		{
			return ESmartDialogueEqualOperation::EEO_Greater;
		}
		if (OperationString == "<")
		{
			return ESmartDialogueEqualOperation::EEO_Less;
		}
		if (OperationString == ">=")
		{
			return ESmartDialogueEqualOperation::EEO_GreaterOrEquals;
		}
		if (OperationString == "<=")
		{
			return ESmartDialogueEqualOperation::EEO_LessOrEquals;
		}

		UE_LOG(LogTemp, Error, TEXT("Invalid OperationString: %s"), *OperationString);
		return ESmartDialogueEqualOperation::EEO_Equals;
	}
};

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Id;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;
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
	int32 Value;
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
struct FSmartDialoguePhrase
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
	TArray<FSmartDialoguePhrase> Phrases;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	bool Choice = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	bool Queue = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	bool Closed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	bool OrCond = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	TArray<FIf> If;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	bool Hidden = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	bool HideSelf = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	TArray<FString> Show;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	TArray<FString> Hide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	FString ChangeStarted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	FSmartDialogueEvent Event;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Branch")
	TArray<FSmartDialogueVars> Vars;
};


USTRUCT(BlueprintType)
struct FDialogueProgress
{
	GENERATED_BODY()

	UPROPERTY()
	FString Auto;
	
	UPROPERTY()
	TArray<FString> Hidden;
	
	UPROPERTY()
	TMap<FName, int32> Queue;
};


USTRUCT(BlueprintType)
struct FCharactersVarsProgress
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FString, int32> Variable;
};

USTRUCT(BlueprintType)
struct FInfoProgress
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FString, FCharactersVarsProgress> CharVars;
	
	UPROPERTY()
	TMap<FString, int32> PublicVars;

	UPROPERTY()
	TMap<FString, FDialogueProgress> Dials;
};
