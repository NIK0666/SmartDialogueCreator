// 
#pragma once

#include "CoreMinimal.h"
#include "SmartDialogueData.h"

#include "SmartDialConfig.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterChanged, int32, Index, const FCharacterData&, CharacterData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCustomParameterChanged, int32, Index, const FCustomParameterData&, CustomParameterData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVariableChanged, int32, Index, const FVariableData&, VariableData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterRemoved, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCustomParameterRemoved, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVariableRemoved, int32, Index);


UCLASS(BlueprintType, Blueprintable)
class SMARTDIALOGUECORE_API USmartDialConfig : public UObject
{
    GENERATED_BODY()
    friend class UEditorDataHelper;
    
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
    // Add new entry methods
    UFUNCTION(BlueprintCallable, Category = "Smart Dial")
    void AddCharacter(const FCharacterData& NewCharacter);

    UFUNCTION(BlueprintCallable, Category = "Smart Dial")
    void AddCustomParameter(const FCustomParameterData& NewCustomParameter);

    UFUNCTION(BlueprintCallable, Category = "Smart Dial")
    void AddVariable(const FVariableData& NewVariable);

    // Remove entry by index methods
    UFUNCTION(BlueprintCallable, Category = "Smart Dial")
    void RemoveCharacterByIndex(int32 Index);

    UFUNCTION(BlueprintCallable, Category = "Smart Dial")
    void RemoveCustomParameterByIndex(int32 Index);

    UFUNCTION(BlueprintCallable, Category = "Smart Dial")
    void RemoveVariableByIndex(int32 Index);

    // Update entry by index methods
    UFUNCTION(BlueprintCallable, Category = "Smart Dial")
    void UpdateCharacterByIndex(int32 Index, const FCharacterData& UpdatedCharacter);

    UFUNCTION(BlueprintCallable, Category = "Smart Dial")
    void UpdateCustomParameterByIndex(int32 Index, const FCustomParameterData& UpdatedCustomParameter);

    UFUNCTION(BlueprintCallable, Category = "Smart Dial")
    void UpdateVariableByIndex(int32 Index, const FVariableData& UpdatedVariable);





    UPROPERTY(BlueprintAssignable, Category = "Smart Dial|Delegates")
    FOnCharacterChanged OnCharacterChanged;

    UPROPERTY(BlueprintAssignable, Category = "Smart Dial|Delegates")
    FOnCustomParameterChanged OnCustomParameterChanged;

    UPROPERTY(BlueprintAssignable, Category = "Smart Dial|Delegates")
    FOnVariableChanged OnVariableChanged;

    UPROPERTY(BlueprintAssignable, Category = "Smart Dial|Delegates")
    FOnCharacterRemoved OnCharacterRemoved;

    UPROPERTY(BlueprintAssignable, Category = "Smart Dial|Delegates")
    FOnCustomParameterRemoved OnCustomParameterRemoved;

    UPROPERTY(BlueprintAssignable, Category = "Smart Dial|Delegates")
    FOnVariableRemoved OnVariableRemoved;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FCharacterData> Characters; //Геттер GetCharacters()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FCustomParameterData> CustomParameters; //Геттер GetCustomParameters()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Hero; //Геттер GetHero()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FVariableData> Variables; //Геттер GetVariables()
};