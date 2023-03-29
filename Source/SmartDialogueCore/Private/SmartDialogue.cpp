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
