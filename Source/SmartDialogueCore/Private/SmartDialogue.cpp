// 


#include "SmartDialogue.h"

#include "SmartDialConfig.h"

TMap<FName, FSmartDialogueBranch> USmartDialogue::GetBranches() const
{
	return Branches;
}

FString USmartDialogue::GetAutoBranch() const
{
	return AutoBranch;
}

FString USmartDialogue::GetCharacter() const
{
	return Character;
}

TArray<FVariableData> USmartDialogue::GetVariables() const
{
	return Variables;
}

void USmartDialogue::SetVariables(const TArray<FVariableData>& NewVariables)
{
	Variables = NewVariables;
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


void USmartDialogue::MakeClean()
{
	Variables = {};
	Branches = {};
	AutoBranch = "";
	Character = "";
}

FString USmartDialogue::GetDialogueId()
{
	return GetName();
}

void USmartDialogue::MoveBranch(const FName& DraggedBranchName, const FName& TargetBranchName)
{
	if (Branches.Contains(DraggedBranchName) && Branches.Contains(TargetBranchName))
	{
		TMap<FName, FSmartDialogueBranch> TempBranches;
		bool bInserted = false;
		bool bDraggedBranchFound = false;

		for (const auto& Entry : Branches)
		{
			if (Entry.Key == DraggedBranchName)
			{
				bDraggedBranchFound = true;
				continue;
			}

			if (Entry.Key == TargetBranchName && !bInserted)
			{
				if (bDraggedBranchFound)
				{
					TempBranches.Add(Entry.Key, Entry.Value);
					TempBranches.Add(DraggedBranchName, Branches[DraggedBranchName]);
				}
				else
				{
					TempBranches.Add(DraggedBranchName, Branches[DraggedBranchName]);
					TempBranches.Add(Entry.Key, Entry.Value);
				}
				bInserted = true;
			}
			else
			{
				TempBranches.Add(Entry.Key, Entry.Value);
			}
		}

		// Если DraggedBranchName находился после TargetBranchName, его необходимо добавить в конец
		if (!bInserted)
		{
			TempBranches.Add(DraggedBranchName, Branches[DraggedBranchName]);
		}

		Branches = TempBranches;
	}
}


void USmartDialogue::MovePhrase(const FName& BranchName, int32 DraggedIndex, int32 TargetIndex)
{
	if (Branches.Contains(BranchName))
	{
		FSmartDialogueBranch* Branch = &Branches[BranchName];
		TArray<FSmartDialoguePhrase>& Phrases = Branch->Phrases;

		if (Phrases.IsValidIndex(DraggedIndex) && Phrases.IsValidIndex(TargetIndex))
		{
			FSmartDialoguePhrase DraggedPhrase = Phrases[DraggedIndex];

			// Если индекс DraggedIndex больше, чем TargetIndex, смещаем элементы вниз
			if (DraggedIndex > TargetIndex)
			{
				Phrases.Insert(DraggedPhrase, TargetIndex);
				Phrases.RemoveAt(DraggedIndex + 1);
			}
			// Если индекс DraggedIndex меньше, чем TargetIndex, смещаем элементы вверх
			else if (DraggedIndex < TargetIndex)
			{
				Phrases.Insert(DraggedPhrase, TargetIndex + 1);
				Phrases.RemoveAt(DraggedIndex);
			}
			// Если индексы равны, ничего не делаем
		}
	}
}

void USmartDialogue::AddHideBranchElement(const FName& BranchName, const FString& Value)
{
	if (Branches.Contains(BranchName))
	{
		auto* BranchPtr = &Branches[BranchName];
		Modify();
		BranchPtr->Hide.Add(Value);
		OnHideBranchAdded.Broadcast(BranchName, Value);
	}
}

void USmartDialogue::RemoveHideBranchElement(const FName& BranchName, int32 Index)
{
	if (Branches.Contains(BranchName))
	{
		auto* BranchPtr = &Branches[BranchName];
		const FString Value = BranchPtr->Hide[Index];
		Modify();
		BranchPtr->Hide.RemoveAt(Index);
		OnHideBranchRemoved.Broadcast(BranchName, Index, Value);
	}
}

void USmartDialogue::UpdateHideBranchElement(const FName& BranchName, int32 Index, const FString& NewValue)
{
	if (Branches.Contains(BranchName))
	{
		auto* BranchPtr = &Branches[BranchName];
		const FString OldValue = BranchPtr->Show[Index];
		if (BranchPtr->Hide.IsValidIndex(Index))
		{
			BranchPtr->Hide[Index] = NewValue;
		}
		OnHideBranchUpdated.Broadcast(BranchName, Index, OldValue, NewValue);
	}
}

void USmartDialogue::RemoveHideBranchByString(const FName& BranchName, const FString& String)
{
	if (Branches.Contains(BranchName))
	{
		int32 Index = Branches[BranchName].Hide.Find(String);
		if (Index != INDEX_NONE)
		{
			RemoveHideBranchElement(BranchName, Index);
		}		
	}
}

void USmartDialogue::AddShowBranchElement(const FName& BranchName, const FString& Value)
{
	if (Branches.Contains(BranchName))
	{
		auto* BranchPtr = &Branches[BranchName];
		Modify();
		BranchPtr->Show.Add(Value);
		OnShowBranchAdded.Broadcast(BranchName, Value);
	}
}

void USmartDialogue::RemoveShowBranchElement(const FName& BranchName, int32 Index)
{
	if (Branches.Contains(BranchName))
	{
		auto* BranchPtr = &Branches[BranchName];
		const FString Value = BranchPtr->Show[Index];
		Modify();
		BranchPtr->Show.RemoveAt(Index);
		OnShowBranchRemoved.Broadcast(BranchName, Index, Value);
	}
}

void USmartDialogue::UpdateShowBranchElement(const FName& BranchName, int32 Index, const FString& NewValue)
{
	if (Branches.Contains(BranchName))
	{
		auto* BranchPtr = &Branches[BranchName];
		const FString OldValue = BranchPtr->Show[Index];
		if (BranchPtr->Show.IsValidIndex(Index))
		{
			Modify();
			BranchPtr->Show[Index] = NewValue;
			OnShowBranchUpdated.Broadcast(BranchName, Index, OldValue, NewValue);
		}
	}
}

void USmartDialogue::RemoveShowBranchByString(const FName& BranchName, const FString& String)
{
	if (Branches.Contains(BranchName))
	{
		int32 Index = Branches[BranchName].Show.Find(String);
		if (Index != INDEX_NONE)
		{
			Modify();
			RemoveShowBranchElement(BranchName, Index);
		}		
	}
}

void USmartDialogue::AddVarElement(const FName& BranchName, const FSmartDialogueVars& NewVar)
{
	if (Branches.Contains(BranchName))
	{
		Modify();
		auto* BranchPtr = &Branches[BranchName];
		BranchPtr->Vars.Add(NewVar);
	}
}

void USmartDialogue::RemoveVarElement(const FName& BranchName, int32 Index)
{
	if (Branches.Contains(BranchName))
	{
		Modify();
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
			Modify();
			BranchPtr->Vars[Index] = Element;
		}
	}
}

void USmartDialogue::AddIfElement(const FName& BranchName, const FIf& Element)
{
	if (Branches.Contains(BranchName))
	{
		Modify();
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
			Modify();
			BranchPtr->If[Index] = Element;
		}
	}
}

void USmartDialogue::RemoveIfElement(const FName& BranchName, int32 Index)
{
	if (Branches.Contains(BranchName))
	{
		Modify();
		auto* BranchPtr = &Branches[BranchName];
		BranchPtr->If.RemoveAt(Index);
	}
}

void USmartDialogue::UpdateEventInfo(const FName& BranchName, const FSmartDialogueEvent& Event)
{
	if (Branches.Contains(BranchName))
	{
		Modify();
		auto* BranchPtr = &Branches[BranchName];
		BranchPtr->Event = Event;
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
		Modify();
		L_BranchPtr->If.Add(NewItem);
	}
}