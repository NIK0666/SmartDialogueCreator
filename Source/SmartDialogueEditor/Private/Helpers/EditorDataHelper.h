// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SmartDialogueData.h"
#include "EditorDataHelper.generated.h"

class FSmartDialogueEditor;

UCLASS()
class SMARTDIALOGUEEDITOR_API UEditorDataHelper : public UObject
{
	GENERATED_BODY()
	friend class USmartDialConfig;
	friend class USmartDialogue;
public:
	// Static methods for modifying USmartDialConfig data

	static void SetHero(FSmartDialogueEditor* SmartDialogueEditor, const FString& NewHero);	
	static void AddCharacter(FSmartDialogueEditor* SmartDialogueEditor, const FCharacterData& NewCharacter);
	static void AddCustomParameter(FSmartDialogueEditor* SmartDialogueEditor, const FCustomParameterData& NewCustomParameter);
	static void AddPublicVariable(FSmartDialogueEditor* SmartDialogueEditor, const FVariableData& NewVariable);
	static void RemoveCharacterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index);
	static void RemoveCustomParameterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index);
	static void RemovePublicVariableByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index);
	static void UpdateCharacterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index, const FCharacterData& UpdatedCharacter);
	static void UpdateCustomParameterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index, const FCustomParameterData& UpdatedCustomParameter);
	static void UpdatePublicVariableByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index, const FVariableData& UpdatedVariable);

	// Static methods for modifying USmartDialogue data

	static void SetAutoBranch(FSmartDialogueEditor* SmartDialogueEditor, const FString& NewAutoBranch);
    static void SetCharacter(FSmartDialogueEditor* SmartDialogueEditor, const FString& NewCharacter);
    static void AddNewBranch(FSmartDialogueEditor* SmartDialogueEditor, FSmartDialogueBranch& NewBranch);
    static void AddNewLocalVariable(FSmartDialogueEditor* SmartDialogueEditor, FVariableData& NewVariable);
    static void RemoveLocalVariableByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index);
    static void UpdateLocalVariableByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index, const FVariableData& VariableData);
    static bool RenameBranch(FSmartDialogueEditor* SmartDialogueEditor, FName OldName, FName NewName);
    static bool RemoveBranch(FSmartDialogueEditor* SmartDialogueEditor, FName BranchName);
    static bool RemoveVarOperation(FSmartDialogueEditor* SmartDialogueEditor, FName BranchName, const int32 Index);
    static bool RemoveIfOperation(FSmartDialogueEditor* SmartDialogueEditor, FName BranchName, int32 Index);
    static void AddVarOperation(FSmartDialogueEditor* SmartDialogueEditor, FName BranchName, const FString& VarName, const FString& OperationString, int32 Value);
    static void AddIfOperation(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, const FString& VarName, const FString& OperationString, int32 Value);
    static void AddHideBranchElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, const FString& Value);
    static void RemoveHideBranchElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, int32 Index);
    static void UpdateHideBranchElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, int32 Index, const FString& NewValue);
    static void RemoveHideBranchByString(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, const FString& String);
    static void AddShowBranchElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, const FString& Value);
    static void RemoveShowBranchElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, int32 Index);
    static void UpdateShowBranchElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, int32 Index, const FString& NewValue);
    static void RemoveShowBranchByString(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, const FString& String);
    static void AddVarElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, const FSmartDialogueVars& NewVar);
    static void RemoveVarElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, int32 Index);
    static void UpdateVarElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, int32 Index, const FSmartDialogueVars& Element);
    static void AddIfElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, const FIf& Element);
    static void UpdateIfElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, int32 Index, const FIf& Element);
    static void RemoveIfElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, int32 Index);
};