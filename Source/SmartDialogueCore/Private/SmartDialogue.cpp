// 


#include "SmartDialogue.h"

TArray<FSmartDialogueBranch> USmartDialogue::GetBranches() const
{
	return Branches;
}

void USmartDialogue::SetBranches(const TArray<FSmartDialogueBranch>& NewBranches)
{
	Branches = NewBranches;
	BranchesChanged();
}

FString USmartDialogue::GetAutoBranch() const
{
	return AutoBranch;
}

void USmartDialogue::SetAutoBranch(const FString& NewAutoBranch)
{
	AutoBranch = NewAutoBranch;
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
