// 


#include "SmartDialogue.h"

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

void USmartDialogue::AddNewBranch(FSmartDialogueBranch& NewBranch)
{
	Branches.Add(NewBranch.Name, NewBranch);
	LastBranchName = NewBranch.Name;
	BranchesChanged();
}

FName USmartDialogue::GenerateBranchName() const
{
	FString LastBranchNameStr = LastBranchName.IsNone() ? "branch_0" : LastBranchName.ToString();
	int32 UnderscoreIndex;
	if (LastBranchNameStr.FindLastChar('_', UnderscoreIndex))
	{
		FString NamePart = LastBranchNameStr.Left(UnderscoreIndex);
		FString NumberPart = LastBranchNameStr.RightChop(UnderscoreIndex + 1);

		if (NumberPart.IsNumeric())
		{
			int32 Number = FCString::Atoi(*NumberPart);
			Number++;

			FName NewName;
			do
			{
				FString FormattedNumber = FString::Printf(TEXT("%0*d"), NumberPart.Len(), Number);
				NewName = FName(*FString::Printf(TEXT("%s_%s"), *NamePart, *FormattedNumber));
				Number++;
			}
			while (Branches.Contains(NewName));

			return NewName;
		}
	}

	// Если что-то пошло не так, вернуть стандартное имя
	return FName("branch_0");
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
