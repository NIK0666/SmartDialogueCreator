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

void USmartDialogue::RemoveBranchInList(bool bIsShowed, const FString& Name)
{
	
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

bool USmartDialogue::RemoveShowBranch(FName BranchName, FString BranchNameString)
{
	if (Branches.Contains(BranchName))
	{
		const auto L_BranchPtr = &Branches[BranchName];
		if (L_BranchPtr->Show.Contains(BranchNameString))
		{
			L_BranchPtr->Show.Remove(BranchNameString);
			return true;
		}		
	}
	return false;
}

bool USmartDialogue::RemoveHideBranch(FName BranchName, FString BranchNameString)
{
	if (Branches.Contains(BranchName))
	{
		const auto L_BranchPtr = &Branches[BranchName];
		if (L_BranchPtr->Hide.Contains(BranchNameString))
		{
			L_BranchPtr->Hide.Remove(BranchNameString);
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
