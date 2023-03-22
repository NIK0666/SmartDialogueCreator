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
	BranchesChanged();
}

void USmartDialogue::BranchesChanged()
{
	OnBranchesChanged.Broadcast();
	UPackage* Package = GetOutermost();
	if (Package && !Package->IsDirty())
	{
		Package->MarkPackageDirty();
	}
}
