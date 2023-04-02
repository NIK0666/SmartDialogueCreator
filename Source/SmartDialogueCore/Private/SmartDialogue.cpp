// 


#include "SmartDialogue.h"

#include "SmartDialConfig.h"
#include "SmartDialogueSettings.h"

TMap<FName, FSmartDialogueBranch> USmartDialogue::GetBranches() const
{
	return Branches;
}

FString USmartDialogue::GetAutoBranch() const
{
	return AutoBranch;
}

void USmartDialogue::SetAutoBranch(const FString& NewAutoBranch)
{
	AutoBranch = NewAutoBranch;
}

FString USmartDialogue::GetCharacter() const
{
	return Character;
}

void USmartDialogue::SetCharacter(const FString& NewCharacter)
{
	Character = NewCharacter;
}

TArray<FVariableData> USmartDialogue::GetVariables() const
{
	return Variables;
}

void USmartDialogue::SetVariables(const TArray<FVariableData>& NewVariables)
{
	Variables = NewVariables;
}

void USmartDialogue::AddNewBranch(FSmartDialogueBranch& NewBranch)
{
	Branches.Add(NewBranch.Name, NewBranch);
	LastBranchName = NewBranch.Name;
	BranchesChanged();
}

void USmartDialogue::AddNewVariable(FVariableData& NewVariable)
{
	Variables.Add(NewVariable);
	//TODO Vars changed!
}

FName USmartDialogue::GenerateBranchName() const
{
	FString BaseName;
	int32 LastNumber = 0;

	if (LastBranchName.IsNone())
	{
		BaseName = "branch";
	}
	else
	{
		FString LastBranchNameStr = LastBranchName.ToString();
		int32 UnderscoreIndex;
		if (LastBranchNameStr.FindLastChar('_', UnderscoreIndex))
		{
			FString NumberPart = LastBranchNameStr.RightChop(UnderscoreIndex + 1);

			if (NumberPart.IsNumeric())
			{
				LastNumber = FCString::Atoi(*NumberPart);
			}
			BaseName = LastBranchNameStr.Left(UnderscoreIndex);
		}
		else
		{
			BaseName = LastBranchNameStr;
		}
	}

	FName NewName;
	do
	{
		FString FormattedNumber = FString::Printf(TEXT("%0*d"), FString::FromInt(LastNumber).Len(), LastNumber);
		NewName = FName(*FString::Printf(TEXT("%s_%s"), *BaseName, *FormattedNumber));
		LastNumber++;
	}
	while (Branches.Contains(NewName));

	return NewName;
}

FSmartDialogueBranch* USmartDialogue::GetBranchPtr(const FName& Name)
{
	if (Branches.Contains(Name))
	{
		return &Branches[Name];
	}

	return nullptr;
}

bool USmartDialogue::RenameBranch(FName OldName, FName NewName)
{
	if (Branches.Contains(OldName) && !Branches.Contains(NewName))
	{
		FSmartDialogueBranch BranchToRename = Branches[OldName];
		BranchToRename.Name = NewName;
		Branches.Remove(OldName);
		Branches.Add(NewName, BranchToRename);
		LastBranchName = NewName;
		MarkAsDirty();
		return true;
	}
	return false;
}

bool USmartDialogue::RemoveBranch(FName BranchName)
{

	FString BranchNameString = BranchName.ToString();

	TArray<FName> BranchKeys;
	Branches.GetKeys(BranchKeys);
	
	for (int32 i = 0; i < BranchKeys.Num(); i++)
	{
		RemoveHideBranch(BranchKeys[i], BranchNameString);
		RemoveShowBranch(BranchKeys[i], BranchNameString);
	}
	Branches.Remove(BranchName);
	return true;
}

void USmartDialogue::AddShowBranch(const FName& BranchName, const FString& AddString)
{
	if (Branches.Contains(BranchName))
	{
		const auto L_BranchPtr = &Branches[BranchName];
		L_BranchPtr->Show.Add(AddString);
	}
}

void USmartDialogue::AddHideBranch(const FName& BranchName, const FString& AddString)
{
	if (Branches.Contains(BranchName))
	{
		const auto L_BranchPtr = &Branches[BranchName];
		L_BranchPtr->Hide.Add(AddString);
	}
}

void USmartDialogue::AddHideBranchElement(const FName& BranchName, int32 Index, const FString& Value)
{
	if (Branches.Contains(BranchName))
	{
		auto* BranchPtr = &Branches[BranchName];
		BranchPtr->Hide.Add(Value);
	}
}

void USmartDialogue::RemoveHideBranchElement(const FName& BranchName, int32 Index)
{
	if (Branches.Contains(BranchName))
	{
		auto* BranchPtr = &Branches[BranchName];
		BranchPtr->Hide.RemoveAt(Index);
	}
}

void USmartDialogue::UpdateHideBranchElement(const FName& BranchName, int32 Index, const FString& NewValue)
{
	if (Branches.Contains(BranchName))
	{
		auto* BranchPtr = &Branches[BranchName];
		if (BranchPtr->Hide.IsValidIndex(Index))
		{
			BranchPtr->Hide[Index] = NewValue;
		}
	}
}

void USmartDialogue::AddShowBranchElement(const FName& BranchName, int32 Index, const FString& Value)
{
	if (Branches.Contains(BranchName))
	{
		auto* BranchPtr = &Branches[BranchName];
		BranchPtr->Show.Add(Value);
	}
}

void USmartDialogue::RemoveShowBranchElement(const FName& BranchName, int32 Index)
{
	if (Branches.Contains(BranchName))
	{
		auto* BranchPtr = &Branches[BranchName];
		BranchPtr->Show.RemoveAt(Index);
	}
}

void USmartDialogue::UpdateShowBranchElement(const FName& BranchName, int32 Index, const FString& NewValue)
{
	if (Branches.Contains(BranchName))
	{
		auto* BranchPtr = &Branches[BranchName];
		if (BranchPtr->Show.IsValidIndex(Index))
		{
			BranchPtr->Show[Index] = NewValue;
		}
	}
}

void USmartDialogue::AddVarElement(const FName& BranchName, int32 Index, const FSmartDialogueVars& NewVar)
{
	if (Branches.Contains(BranchName))
	{
		auto* BranchPtr = &Branches[BranchName];
		BranchPtr->Vars.Add(NewVar);
	}
}

void USmartDialogue::RemoveVarElement(const FName& BranchName, int32 Index)
{
	if (Branches.Contains(BranchName))
	{
		auto* BranchPtr = &Branches[BranchName];
		BranchPtr->Vars.RemoveAt(Index);
	}
}

void USmartDialogue::UpdateVarElement(const FName& BranchName, int32 Index, const FSmartDialogueVars& Element)
{
	if (Branches.Contains(BranchName))
	{
		auto* BranchPtr = &Branches[BranchName];
		if (BranchPtr->Vars.IsValidIndex(Index))
		{
			BranchPtr->Vars[Index] = Element;
		}
	}
}

void USmartDialogue::AddIfElement(const FName& BranchName, int32 Index, const FIf& Element)
{
	if (Branches.Contains(BranchName))
	{
		auto* BranchPtr = &Branches[BranchName];
		BranchPtr->If.Add(Element);
	}
}

void USmartDialogue::UpdateIfElement(const FName& BranchName, int32 Index, const FIf& Element)
{
	if (Branches.Contains(BranchName))
	{
		auto* BranchPtr = &Branches[BranchName];
		if (BranchPtr->If.IsValidIndex(Index))
		{
			BranchPtr->If[Index] = Element;
		}
	}
}

void USmartDialogue::RemoveIfElement(const FName& BranchName, int32 Index)
{
	if (Branches.Contains(BranchName))
	{
		auto* BranchPtr = &Branches[BranchName];
		BranchPtr->If.RemoveAt(Index);
	}
}

bool USmartDialogue::RemoveShowBranch(FName BranchName, const FString& RemoveString)
{
	if (Branches.Contains(BranchName))
	{
		const auto L_BranchPtr = &Branches[BranchName];		
		if (L_BranchPtr->Show.Contains(RemoveString))
		{
			L_BranchPtr->Show.Remove(RemoveString);
			return true;
		}
	}
	return false;
}

bool USmartDialogue::RemoveVarOperation(FName BranchName, const int32 Index)
{
	if (Branches.Contains(BranchName))
	{
		const auto L_BranchPtr = &Branches[BranchName];
		L_BranchPtr->Vars.RemoveAt(Index);
		return true;
	}
	return false;
}

bool USmartDialogue::RemoveIfOperation(FName BranchName, const int32 Index)
{
	if (Branches.Contains(BranchName))
	{
		const auto L_BranchPtr = &Branches[BranchName];
		L_BranchPtr->If.RemoveAt(Index);
		return true;
	}
	return false;
}

void USmartDialogue::AddVarOperation(FName BranchName, const FString& VarName, const FString& OperationString, int32 Value)
{
	if (Branches.Contains(BranchName))
	{
		const auto L_BranchPtr = &Branches[BranchName];

		FSmartDialogueVars NewItem;
		NewItem.Key = VarName;
		NewItem.Value = Value;

		if (OperationString == "=")
		{
			NewItem.Operation = ESmartDialogueOperation::EO_Equals;
		}
		else if (OperationString == "+")
		{
			NewItem.Operation = ESmartDialogueOperation::EO_Plus;
		}
		else if (OperationString == "-")
		{
			NewItem.Operation = ESmartDialogueOperation::EO_Minus;
		}
		else if (OperationString == "*")
		{
			NewItem.Operation = ESmartDialogueOperation::EO_Multiply;
		}
		else if (OperationString == "/")
		{
			NewItem.Operation = ESmartDialogueOperation::EO_Divide;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid OperationString: %s"), *OperationString);
			return;
		}

		L_BranchPtr->Vars.Add(NewItem);
	}
}

void USmartDialogue::AddIfOperation(const FName& BranchName, const FString& VarName, const FString& OperationString, int32 Value)
{
	if (Branches.Contains(BranchName))
	{
		const auto L_BranchPtr = &Branches[BranchName];

		FIf NewItem;
		NewItem.Key = VarName;
		NewItem.Value = Value;

		if (OperationString == "==")
		{
			NewItem.EqualOperation = ESmartDialogueEqualOperation::EEO_Equals;
		}
		else if (OperationString == ">=")
		{
			NewItem.EqualOperation = ESmartDialogueEqualOperation::EEO_GreaterOrEquals;
		}
		else if (OperationString == "<=")
		{
			NewItem.EqualOperation = ESmartDialogueEqualOperation::EEO_LessOrEquals;
		}
		else if (OperationString == ">")
		{
			NewItem.EqualOperation = ESmartDialogueEqualOperation::EEO_Greater;
		}
		else if (OperationString == "<")
		{
			NewItem.EqualOperation = ESmartDialogueEqualOperation::EEO_Less;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid OperationString: %s"), *OperationString);
			return;
		}

		L_BranchPtr->If.Add(NewItem);
	}
}

bool USmartDialogue::RemoveHideBranch(FName BranchName, const FString& RemoveString)
{
	if (Branches.Contains(BranchName))
	{
		const auto L_BranchPtr = &Branches[BranchName];
		if (L_BranchPtr->Hide.Contains(RemoveString))
		{
			L_BranchPtr->Hide.Remove(RemoveString);
			return true;
		}		
	}
	return false;
}

void USmartDialogue::BranchesChanged()
{
	OnBranchesChanged.Broadcast();
	MarkAsDirty();
}

void USmartDialogue::MarkAsDirty()
{
	UPackage* Package = GetOutermost();
	if (Package && !Package->IsDirty())
	{
		Package->MarkPackageDirty();
	}
}
