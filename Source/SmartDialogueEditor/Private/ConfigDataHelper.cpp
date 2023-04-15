// 


#include "ConfigDataHelper.h"
#include "ScopedTransaction.h"
#include "Toolkit/FSmartDialogueEditor.h"
#define LOCTEXT_NAMESPACE "UConfigDataHelper"

void UConfigDataHelper::SetCharacters(FSmartDialogueEditor* SmartDialogueEditor, const TArray<FCharacterData>& NewCharacters)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Set Characters"), LOCTEXT("SetCharacters", "Set Characters"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->SetCharacters(NewCharacters);
	}
}

void UConfigDataHelper::SetCustomParameters(FSmartDialogueEditor* SmartDialogueEditor, const TArray<FCustomParameterData>& NewCustomParameters)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Set Custom Parameters"), LOCTEXT("SetCustomParameters", "Set Custom Parameters"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->SetCustomParameters(NewCustomParameters);
	}
}

void UConfigDataHelper::SetHero(FSmartDialogueEditor* SmartDialogueEditor, const FString& NewHero)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Set Hero"), LOCTEXT("SetHero", "Set Hero"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->SetHero(NewHero);
	}
}

void UConfigDataHelper::SetVariables(FSmartDialogueEditor* SmartDialogueEditor, const TArray<FVariableData>& NewVariables)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Set Variables"), LOCTEXT("SetVariables", "Set Variables"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->SetVariables(NewVariables);
	}
}

void UConfigDataHelper::AddCharacter(FSmartDialogueEditor* SmartDialogueEditor, const FCharacterData& NewCharacter)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add Character"), LOCTEXT("AddCharacter", "Add Character"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->AddCharacter(NewCharacter);
	}
}

void UConfigDataHelper::AddCustomParameter(FSmartDialogueEditor* SmartDialogueEditor, const FCustomParameterData& NewCustomParameter)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add Custom Parameter"), LOCTEXT("AddCustomParameter", "Add Custom Parameter"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->AddCustomParameter(NewCustomParameter);
	}
}

void UConfigDataHelper::AddVariable(FSmartDialogueEditor* SmartDialogueEditor, const FVariableData& NewVariable)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add Variable"), LOCTEXT("AddVariable", "Add Variable"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->AddVariable(NewVariable);
	}
}

void UConfigDataHelper::RemoveCharacterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Character"), LOCTEXT("RemoveCharacter", "Remove Character"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->RemoveCharacterByIndex(Index);
	}
}

void UConfigDataHelper::RemoveCustomParameterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Custom Parameter"), LOCTEXT("RemoveCustomParameter", "Remove Custom Parameter"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->RemoveCustomParameterByIndex(Index);
	}
}


void UConfigDataHelper::RemoveVariableByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Variable"), LOCTEXT("RemoveVariable", "Remove Variable"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->RemoveVariableByIndex(Index);
	}
}

void UConfigDataHelper::UpdateCharacterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index, const FCharacterData& UpdatedCharacter)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Update Character"), LOCTEXT("UpdateCharacter", "Update Character"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->UpdateCharacterByIndex(Index, UpdatedCharacter);
	}
}

void UConfigDataHelper::UpdateCustomParameterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index, const FCustomParameterData& UpdatedCustomParameter)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Update Custom Parameter"), LOCTEXT("UpdateCustomParameter", "Update Custom Parameter"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->UpdateCustomParameterByIndex(Index, UpdatedCustomParameter);
	}
}

void UConfigDataHelper::UpdateVariableByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index, const FVariableData& UpdatedVariable)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Update Variable"), LOCTEXT("UpdateVariable", "Update Variable"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->UpdateVariableByIndex(Index, UpdatedVariable);
	}
}

#undef LOCTEXT_NAMESPACE