// 
#pragma once

#include "CoreMinimal.h"
#include "SmartDialogueData.h"

#include "SmartDialConfig.generated.h"

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
    TArray<FCharacterData> Characters; //Геттер GetCharacters()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FCustomParameterData> CustomParameters; //Геттер GetCustomParameters()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Hero; //Геттер GetHero()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FVariableData> Variables; //Геттер GetVariables()
};