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

FName USmartDialogue::GenerateBranchName() const
{
	int32 MaxIndex = -1;

	for (const auto& KeyValue : Branches)
	{
		FString KeyStr = KeyValue.Key.ToString();
		int32 UnderscoreIndex;
		if (KeyStr.FindLastChar('_', UnderscoreIndex))
		{
			FString NamePart = KeyStr.Left(UnderscoreIndex);
			FString NumberPart = KeyStr.RightChop(UnderscoreIndex + 1);

			if (NumberPart.IsNumeric())
			{
				int32 Number = FCString::Atoi(*NumberPart);
				if (Number > MaxIndex)
				{
					MaxIndex = Number;
				}
			}
		}
	}

	MaxIndex++;

	FString BaseName = "branch";
	return FName(*FString::Printf(TEXT("%s_%d"), *BaseName, MaxIndex));
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