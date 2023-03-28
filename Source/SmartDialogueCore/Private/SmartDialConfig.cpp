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
