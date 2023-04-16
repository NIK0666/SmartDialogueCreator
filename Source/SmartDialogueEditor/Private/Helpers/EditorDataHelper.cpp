// 


#include "EditorDataHelper.h"
#include "ScopedTransaction.h"
#include "SmartDialogue.h"
#include "Toolkit/FSmartDialogueEditor.h"
#define LOCTEXT_NAMESPACE "UEditorDataHelper"

void UEditorDataHelper::SetHero(FSmartDialogueEditor* SmartDialogueEditor, const FString& NewHero)
{
	
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Set Hero"), LOCTEXT("SetHero", "Set Hero"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->SetHero(NewHero);
	}
}

void UEditorDataHelper::AddCharacter(FSmartDialogueEditor* SmartDialogueEditor, const FCharacterData& NewCharacter)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add Character"), LOCTEXT("AddCharacter", "Add Character"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->AddCharacter(NewCharacter);
	}
}

void UEditorDataHelper::AddCustomParameter(FSmartDialogueEditor* SmartDialogueEditor, const FCustomParameterData& NewCustomParameter)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add Custom Parameter"), LOCTEXT("AddCustomParameter", "Add Custom Parameter"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->AddCustomParameter(NewCustomParameter);
	}
}

void UEditorDataHelper::AddVariable(FSmartDialogueEditor* SmartDialogueEditor, const FVariableData& NewVariable)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add Variable"), LOCTEXT("AddVariable", "Add Variable"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->AddVariable(NewVariable);
	}
}

void UEditorDataHelper::RemoveCharacterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Character"), LOCTEXT("RemoveCharacter", "Remove Character"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->RemoveCharacterByIndex(Index);
	}
}

void UEditorDataHelper::RemoveCustomParameterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Custom Parameter"), LOCTEXT("RemoveCustomParameter", "Remove Custom Parameter"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->RemoveCustomParameterByIndex(Index);
	}
}


void UEditorDataHelper::RemoveVariableByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Variable"), LOCTEXT("RemoveVariable", "Remove Variable"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->RemoveVariableByIndex(Index);
	}
}

void UEditorDataHelper::UpdateCharacterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index, const FCharacterData& UpdatedCharacter)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Update Character"), LOCTEXT("UpdateCharacter", "Update Character"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->UpdateCharacterByIndex(Index, UpdatedCharacter);
	}
}

void UEditorDataHelper::UpdateCustomParameterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index, const FCustomParameterData& UpdatedCustomParameter)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Update Custom Parameter"), LOCTEXT("UpdateCustomParameter", "Update Custom Parameter"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->UpdateCustomParameterByIndex(Index, UpdatedCustomParameter);
	}
}

void UEditorDataHelper::UpdateVariableByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index, const FVariableData& UpdatedVariable)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Update Variable"), LOCTEXT("UpdateVariable", "Update Variable"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->UpdateVariableByIndex(Index, UpdatedVariable);
	}
}

void UEditorDataHelper::SetAutoBranch(FSmartDialogueEditor* SmartDialogueEditor, const FString& NewAutoBranch)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Set AutoBranch"), LOCTEXT("SetAutoBranch", "Set AutoBranch"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->SetAutoBranch(NewAutoBranch);
	}
}

void UEditorDataHelper::SetCharacter(FSmartDialogueEditor* SmartDialogueEditor, const FString& NewCharacter)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Set Character"), LOCTEXT("SetCharacter", "Set Character"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->SetCharacter(NewCharacter);
	}
}

void UEditorDataHelper::AddNewBranch(FSmartDialogueEditor* SmartDialogueEditor, FSmartDialogueBranch& NewBranch)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add New Branch"), LOCTEXT("AddNewBranch", "Add New Branch"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->AddNewBranch(NewBranch);
	}
}

void UEditorDataHelper::AddNewLocalVariable(FSmartDialogueEditor* SmartDialogueEditor, FVariableData& NewVariable)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add New Local Variable"), LOCTEXT("AddNewLocalVariable", "Add New Local Variable"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->AddNewVariable(NewVariable);
	}
}

void UEditorDataHelper::RemoveLocalVariableByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Local Variable"), LOCTEXT("RemoveLocalVariable", "Remove Local Variable"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->RemoveVariableByIndex(Index);
	}
}

void UEditorDataHelper::UpdateLocalVariableByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index,
	const FVariableData& VariableData)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Update Local Variable"), LOCTEXT("UpdateLocalVariable", "Update Local Variable"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->UpdateVariableByIndex(Index, VariableData);
	}
}

bool UEditorDataHelper::RenameBranch(FSmartDialogueEditor* SmartDialogueEditor, FName OldName, FName NewName)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Rename Branch"), LOCTEXT("RenameBranch", "Rename Branch"), SmartDialogueEditor->GetDialogue());
		return SmartDialogueEditor->GetDialogue()->RenameBranch(OldName, NewName);
	}
	return false;
}

bool UEditorDataHelper::RemoveBranch(FSmartDialogueEditor* SmartDialogueEditor, FName BranchName)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Branch"), LOCTEXT("RemoveBranch", "Remove Branch"), SmartDialogueEditor->GetDialogue());
		return SmartDialogueEditor->GetDialogue()->RemoveBranch(BranchName);
	}
	return false;
}

bool UEditorDataHelper::RemoveVarOperation(FSmartDialogueEditor* SmartDialogueEditor, FName BranchName,
	const int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Var Operation"), LOCTEXT("RemoveVarOperation", "Remove Var Operation"), SmartDialogueEditor->GetDialogue());
		return SmartDialogueEditor->GetDialogue()->RemoveVarOperation(BranchName, Index);
	}
	return false;
}

bool UEditorDataHelper::RemoveIfOperation(FSmartDialogueEditor* SmartDialogueEditor, FName BranchName, int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove If Operation"), LOCTEXT("RemoveIfOperation", "Remove If Operation"), SmartDialogueEditor->GetDialogue());
		return SmartDialogueEditor->GetDialogue()->RemoveIfOperation(BranchName, Index);
	}
	return false;
}

void UEditorDataHelper::AddVarOperation(FSmartDialogueEditor* SmartDialogueEditor, FName BranchName,
	const FString& VarName, const FString& OperationString, int32 Value)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add Var Operation"), LOCTEXT("AddVarOperation", "Add Var Operation"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->AddVarOperation(BranchName, VarName, OperationString, Value);
	}
}

void UEditorDataHelper::AddIfOperation(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	const FString& VarName, const FString& OperationString, int32 Value)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add If Operation"), LOCTEXT("AddIfOperation", "Add If Operation"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->AddIfOperation(BranchName, VarName, OperationString, Value);
	}
}

void UEditorDataHelper::AddHideBranchElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	const FString& Value)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add Hide Branch Element"), LOCTEXT("AddHideBranchElement", "Add Hide Branch Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->AddHideBranchElement(BranchName, Value);
	}
}

void UEditorDataHelper::RemoveHideBranchElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Hide Branch"), LOCTEXT("RemoveHideBranch", "Remove Hide Branch"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->RemoveHideBranchElement(BranchName, Index);
	}
}

void UEditorDataHelper::UpdateHideBranchElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	int32 Index, const FString& NewValue)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Update Hide Branch"), LOCTEXT("UpdateHideBranch", "Update Hide Branch"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->UpdateHideBranchElement(BranchName, Index, NewValue);
	}
}

void UEditorDataHelper::RemoveHideBranchByString(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	const FString& String)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Hide Branch"), LOCTEXT("RemoveHideBranch", "Remove Hide Branch"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->RemoveHideBranchByString(BranchName, String);
	}
}

void UEditorDataHelper::AddShowBranchElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	const FString& Value)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add Show Branch Element"), LOCTEXT("AddShowBranchElement", "Add Show Branch Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->AddShowBranchElement(BranchName, Value);
	}
}

void UEditorDataHelper::RemoveShowBranchElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Show Branch Element"), LOCTEXT("RemoveShowBranchElement", "Remove Show Branch Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->RemoveShowBranchElement(BranchName, Index);
	}
}

void UEditorDataHelper::UpdateShowBranchElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	int32 Index, const FString& NewValue)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Update Show Branch Element"), LOCTEXT("UpdateShowBranchElement", "Update Show Branch Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->UpdateShowBranchElement(BranchName, Index, NewValue);
	}
}

void UEditorDataHelper::RemoveShowBranchByString(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	const FString& String)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Show Branch By String"), LOCTEXT("RemoveShowBranchByString", "Remove Show Branch By String"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->RemoveShowBranchByString(BranchName, String);
	}
}

void UEditorDataHelper::AddVarElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	const FSmartDialogueVars& NewVar)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add Var Element"), LOCTEXT("AddVarElement", "Add Var Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->AddVarElement(BranchName, NewVar);
	}
}

void UEditorDataHelper::RemoveVarElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Var Element"), LOCTEXT("RemoveVarElement", "Remove Var Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->RemoveVarElement(BranchName, Index);
	}
}

void UEditorDataHelper::UpdateVarElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	int32 Index, const FSmartDialogueVars& Element)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Update Var Element"), LOCTEXT("UpdateVarElement", "Update Var Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->UpdateVarElement(BranchName, Index, Element);
	}
}

void UEditorDataHelper::AddIfElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	const FIf& Element)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add If Element"), LOCTEXT("AddIfElement", "Add If Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->AddIfElement(BranchName, Element);
	}
}

void UEditorDataHelper::UpdateIfElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, int32 Index,
	const FIf& Element)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Update If Element"), LOCTEXT("UpdateIfElement", "Update If Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->UpdateIfElement(BranchName, Index, Element);
	}
}

void UEditorDataHelper::RemoveIfElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove If Element"), LOCTEXT("RemoveIfElement", "Remove If Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->RemoveIfElement(BranchName, Index);
	}
}

#undef LOCTEXT_NAMESPACE
