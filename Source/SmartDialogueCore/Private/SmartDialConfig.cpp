// 


#include "SmartDialConfig.h"

TArray<FCharacterData> USmartDialConfig::GetCharacters() const
{
	return Characters;
}

void USmartDialConfig::SetCharacters(const TArray<FCharacterData>& NewCharacters)
{
	Characters = NewCharacters;
}

TArray<FCustomParameterData> USmartDialConfig::GetCustomParameters() const
{
	return CustomParameters;
}

void USmartDialConfig::SetCustomParameters(const TArray<FCustomParameterData>& NewCustomParameters)
{
	CustomParameters = NewCustomParameters;
}

FString USmartDialConfig::GetHero() const
{
	return Hero;
}

void USmartDialConfig::SetHero(const FString& NewHero)
{
	Hero = NewHero;
}

TArray<FVariableData> USmartDialConfig::GetVariables() const
{
	return Variables;
}

void USmartDialConfig::SetVariables(const TArray<FVariableData>& NewVariables)
{
	Variables = NewVariables;
}

void USmartDialConfig::AddCharacter(const FCharacterData& NewCharacter)
{
	Modify();
	const int32 Index = Characters.Add(NewCharacter);
	OnCharacterChanged.Broadcast(Index, NewCharacter);
}

void USmartDialConfig::AddCustomParameter(const FCustomParameterData& NewCustomParameter)
{
	Modify();
	const int32 Index = CustomParameters.Add(NewCustomParameter);
	OnCustomParameterChanged.Broadcast(Index, NewCustomParameter);
}

void USmartDialConfig::AddVariable(const FVariableData& NewVariable)
{
	Modify();
	const int32 Index = Variables.Add(NewVariable);
	OnVariableChanged.Broadcast(Index, NewVariable);
}

void USmartDialConfig::RemoveCharacterByIndex(int32 Index)
{
	if (Characters.IsValidIndex(Index))
	{
		Modify();
		Characters.RemoveAt(Index);
		OnCharacterRemoved.Broadcast(Index);
	}
}

void USmartDialConfig::RemoveCustomParameterByIndex(int32 Index)
{
	if (CustomParameters.IsValidIndex(Index))
	{
		Modify();
		CustomParameters.RemoveAt(Index);
		OnCustomParameterRemoved.Broadcast(Index);
	}
}

void USmartDialConfig::RemoveVariableByIndex(int32 Index)
{
	if (Variables.IsValidIndex(Index))
	{
		Modify();
		Variables.RemoveAt(Index);
		OnVariableRemoved.Broadcast(Index);
	}
}

void USmartDialConfig::UpdateCharacterByIndex(int32 Index, const FCharacterData& UpdatedCharacter)
{
	if (Characters.IsValidIndex(Index))
	{
		Modify();
		Characters[Index] = UpdatedCharacter;
		OnCharacterChanged.Broadcast(Index, UpdatedCharacter);
	}
}

void USmartDialConfig::UpdateCustomParameterByIndex(int32 Index, const FCustomParameterData& UpdatedCustomParameter)
{
	if (CustomParameters.IsValidIndex(Index))
	{
		Modify();
		CustomParameters[Index] = UpdatedCustomParameter;
		OnCustomParameterChanged.Broadcast(Index, UpdatedCustomParameter);
	}
}

void USmartDialConfig::UpdateVariableByIndex(int32 Index, const FVariableData& UpdatedVariable)
{
	if (Variables.IsValidIndex(Index))
	{
		Modify();
		Variables[Index] = UpdatedVariable;
		OnVariableChanged.Broadcast(Index, UpdatedVariable);
	}
}
