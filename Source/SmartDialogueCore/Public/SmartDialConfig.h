// 
#pragma once

#include "CoreMinimal.h"
#include "SmartDialConfig.generated.h"

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

UCLASS(BlueprintType, Blueprintable)
class SMARTDIALOGUECORE_API USmartDialConfig : public UObject
{
    GENERATED_BODY()

public:
    // Getter and Setter for Characters
    UFUNCTION(BlueprintPure, Category = "Smart Dial")
    TArray<FCharacterData> GetCharacters() const;

    UFUNCTION(BlueprintCallable, Category = "Smart Dial")
    void SetCharacters(const TArray<FCharacterData>& NewCharacters);

    // Getter and Setter for CustomParameters
    UFUNCTION(BlueprintPure, Category = "Smart Dial")
    TArray<FCustomParameterData> GetCustomParameters() const;

    UFUNCTION(BlueprintCallable, Category = "Smart Dial")
    void SetCustomParameters(const TArray<FCustomParameterData>& NewCustomParameters);

    // Getter and Setter for Hero
    UFUNCTION(BlueprintPure, Category = "Smart Dial")
    FString GetHero() const;

    UFUNCTION(BlueprintCallable, Category = "Smart Dial")
    void SetHero(const FString& NewHero);

    // Getter and Setter for Variables
    UFUNCTION(BlueprintPure, Category = "Smart Dial")
    TArray<FVariableData> GetVariables() const;

    UFUNCTION(BlueprintCallable, Category = "Smart Dial")
    void SetVariables(const TArray<FVariableData>& NewVariables);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FCharacterData> Characters;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FCustomParameterData> CustomParameters;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Hero;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FVariableData> Variables;
};