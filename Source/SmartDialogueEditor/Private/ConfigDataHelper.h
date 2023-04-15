// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SmartDialogueData.h"
#include "ConfigDataHelper.generated.h"

class FSmartDialogueEditor;

UCLASS()
class SMARTDIALOGUEEDITOR_API UConfigDataHelper : public UObject
{
	GENERATED_BODY()

public:
	// Static methods for modifying USmartDialConfig data

	static void SetCharacters(FSmartDialogueEditor* SmartDialogueEditor, const TArray<FCharacterData>& NewCharacters);

	static void SetCustomParameters(FSmartDialogueEditor* SmartDialogueEditor, const TArray<FCustomParameterData>& NewCustomParameters);

	static void SetHero(FSmartDialogueEditor* SmartDialogueEditor, const FString& NewHero);

	static void SetVariables(FSmartDialogueEditor* SmartDialogueEditor, const TArray<FVariableData>& NewVariables);



	static void AddCharacter(FSmartDialogueEditor* SmartDialogueEditor, const FCharacterData& NewCharacter);

	static void AddCustomParameter(FSmartDialogueEditor* SmartDialogueEditor, const FCustomParameterData& NewCustomParameter);

	static void AddVariable(FSmartDialogueEditor* SmartDialogueEditor, const FVariableData& NewVariable);

	static void RemoveCharacterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index);

	static void RemoveCustomParameterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index);

	static void RemoveVariableByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index);

	static void UpdateCharacterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index, const FCharacterData& UpdatedCharacter);

	static void UpdateCustomParameterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index, const FCustomParameterData& UpdatedCustomParameter);

	static void UpdateVariableByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index, const FVariableData& UpdatedVariable);

};