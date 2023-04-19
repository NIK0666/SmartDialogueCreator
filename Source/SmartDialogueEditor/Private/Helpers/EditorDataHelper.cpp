// 


#include "EditorDataHelper.h"
#include "ScopedTransaction.h"
#include "SmartDialogue.h"
#include "Toolkit/FSmartDialogueEditor.h"
#define LOCTEXT_NAMESPACE "UEditorDataHelper"

void UEditorDataHelper::SetHero(FSmartDialogueEditor* SmartDialogueEditor, const FString& NewHero)
{
	
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Set Hero"), LOCTEXT("SetHero", "Set Hero"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->SetHero(NewHero);
	}
}

void UEditorDataHelper::AddCharacter(FSmartDialogueEditor* SmartDialogueEditor, const FCharacterData& NewCharacter)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add Character"), LOCTEXT("AddCharacter", "Add Character"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->AddCharacter(NewCharacter);
	}
}

void UEditorDataHelper::AddCustomParameter(FSmartDialogueEditor* SmartDialogueEditor, const FCustomParameterData& NewCustomParameter)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add Custom Parameter"), LOCTEXT("AddCustomParameter", "Add Custom Parameter"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->AddCustomParameter(NewCustomParameter);
	}
}

void UEditorDataHelper::AddPublicVariable(FSmartDialogueEditor* SmartDialogueEditor, const FVariableData& NewVariable)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add Public Variable"), LOCTEXT("AddPublicVariable", "Add Public Variable"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->AddVariable(NewVariable);
	}
}

void UEditorDataHelper::RemoveCharacterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Character"), LOCTEXT("RemoveCharacter", "Remove Character"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->RemoveCharacterByIndex(Index);
	}
}

void UEditorDataHelper::RemoveCustomParameterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Custom Parameter"), LOCTEXT("RemoveCustomParameter", "Remove Custom Parameter"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->RemoveCustomParameterByIndex(Index);
	}
}


void UEditorDataHelper::RemovePublicVariableByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Variable"), LOCTEXT("RemoveVariable", "Remove Variable"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->RemoveVariableByIndex(Index);
	}
}

void UEditorDataHelper::UpdateCharacterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index, const FCharacterData& UpdatedCharacter)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Update Character"), LOCTEXT("UpdateCharacter", "Update Character"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->UpdateCharacterByIndex(Index, UpdatedCharacter);
	}
}

void UEditorDataHelper::UpdateCustomParameterByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index, const FCustomParameterData& UpdatedCustomParameter)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Update Custom Parameter"), LOCTEXT("UpdateCustomParameter", "Update Custom Parameter"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->UpdateCustomParameterByIndex(Index, UpdatedCustomParameter);
	}
}

void UEditorDataHelper::UpdatePublicVariableByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index, const FVariableData& UpdatedVariable)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Update Variable"), LOCTEXT("UpdateVariable", "Update Variable"), SmartDialogueEditor->GetDialogueConfig());
		SmartDialogueEditor->GetDialogueConfig()->UpdateVariableByIndex(Index, UpdatedVariable);
	}
}

void UEditorDataHelper::SetAutoBranch(FSmartDialogueEditor* SmartDialogueEditor, const FString& NewAutoBranch)
{
	if (SmartDialogueEditor)
	{
		USmartDialogue* SmartDialogue = SmartDialogueEditor->GetDialogue();
		FScopedTransaction Transaction(TEXT("Set AutoBranch"), LOCTEXT("SetAutoBranch", "Set AutoBranch"), SmartDialogue);
		SmartDialogue->Modify();
		SmartDialogue->AutoBranch = NewAutoBranch;
	}
}

void UEditorDataHelper::SetCharacter(FSmartDialogueEditor* SmartDialogueEditor, const FString& NewCharacter)
{
	if (SmartDialogueEditor)
	{
		USmartDialogue* SmartDialogue = SmartDialogueEditor->GetDialogue();
		FScopedTransaction Transaction(TEXT("Set Character"), LOCTEXT("SetCharacter", "Set Character"), SmartDialogue);
		SmartDialogue->Modify();
		SmartDialogue->Character = NewCharacter;
	}
}

void UEditorDataHelper::AddNewBranch(FSmartDialogueEditor* SmartDialogueEditor, FSmartDialogueBranch& NewBranch)
{
	if (SmartDialogueEditor)
	{
		USmartDialogue* SmartDialogue = SmartDialogueEditor->GetDialogue();
		FScopedTransaction Transaction(TEXT("Add New Branch"), LOCTEXT("AddNewBranch", "Add New Branch"), SmartDialogue);
		SmartDialogue->Modify();
		SmartDialogue->Branches.Add(NewBranch.Name, NewBranch);
		SmartDialogue->LastBranchName = NewBranch.Name;
		SmartDialogue->OnBranchesChanged.Broadcast();
	}
}

void UEditorDataHelper::AddNewLocalVariable(FSmartDialogueEditor* SmartDialogueEditor, FVariableData& NewVariable)
{
	if (SmartDialogueEditor)
	{
		USmartDialogue* SmartDialogue = SmartDialogueEditor->GetDialogue();
		FScopedTransaction Transaction(TEXT("Add New Local Variable"), LOCTEXT("AddNewLocalVariable", "Add New Local Variable"), SmartDialogue);
		SmartDialogue->Modify();
		SmartDialogue->Variables.Add(NewVariable);
		//TODO Vars changed!
	}
}

void UEditorDataHelper::RemoveLocalVariableByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index)
{
	if (SmartDialogueEditor)
	{
		USmartDialogue* SmartDialogue = SmartDialogueEditor->GetDialogue();
		FScopedTransaction Transaction(TEXT("Remove Local Variable"), LOCTEXT("RemoveLocalVariable", "Remove Local Variable"), SmartDialogue);
		SmartDialogue->Modify();
		SmartDialogue->Variables.RemoveAt(Index);
	}
}

void UEditorDataHelper::UpdateLocalVariableByIndex(FSmartDialogueEditor* SmartDialogueEditor, int32 Index,
	const FVariableData& VariableData)
{
	if (SmartDialogueEditor)
	{
		USmartDialogue* SmartDialogue = SmartDialogueEditor->GetDialogue();
		FScopedTransaction Transaction(TEXT("Update Local Variable"), LOCTEXT("UpdateLocalVariable", "Update Local Variable"), SmartDialogue);
		SmartDialogue->Modify();
		SmartDialogue->Variables[Index] = VariableData;
	}
}

bool UEditorDataHelper::RenameBranch(FSmartDialogueEditor* SmartDialogueEditor, FName OldName, FName NewName)
{
	if (SmartDialogueEditor)
	{
		USmartDialogue* SmartDialogue = SmartDialogueEditor->GetDialogue();
		FScopedTransaction Transaction(TEXT("Rename Branch"), LOCTEXT("RenameBranch", "Rename Branch"), SmartDialogue);

		if (SmartDialogue->Branches.Contains(OldName) && !SmartDialogue->Branches.Contains(NewName))
		{
			FSmartDialogueBranch BranchToRename = SmartDialogue->Branches[OldName];
			SmartDialogue->Modify();
			BranchToRename.Name = NewName;
			SmartDialogue->Branches.Remove(OldName);
			SmartDialogue->Branches.Add(NewName, BranchToRename);
			SmartDialogue->LastBranchName = NewName;

			for (auto Element : SmartDialogue->Branches)
			{
				auto HideIndex = Element.Value.Hide.Find(OldName.ToString());
				if (HideIndex != INDEX_NONE)
				{
					SmartDialogue->Branches[Element.Key].Hide[HideIndex] = NewName.ToString();
				}
				auto ShowIndex = Element.Value.Show.Find(OldName.ToString());
				if (ShowIndex != INDEX_NONE)
				{
					SmartDialogue->Branches[Element.Key].Show[ShowIndex] = NewName.ToString();
				}

				if (Element.Value.ChangeStarted == OldName.ToString())
				{
					SmartDialogue->Branches[Element.Key].ChangeStarted = NewName.ToString();
				}
			}

			if (SmartDialogue->AutoBranch == OldName.ToString())
			{
				SmartDialogue->AutoBranch = NewName.ToString();
			}

			SmartDialogue->OnBranchRenamed.Broadcast(OldName, NewName);

			return true;
		}
	}
	return false;
}

bool UEditorDataHelper::RemoveBranch(FSmartDialogueEditor* SmartDialogueEditor, FName BranchName)
{
	if (SmartDialogueEditor)
	{
		USmartDialogue* SmartDialogue = SmartDialogueEditor->GetDialogue();
		FScopedTransaction Transaction(TEXT("Remove Branch"), LOCTEXT("RemoveBranch", "Remove Branch"), SmartDialogue);

		FString BranchNameString = BranchName.ToString();

		TArray<FName> BranchKeys;
		SmartDialogue->Branches.GetKeys(BranchKeys);
		SmartDialogue->Modify();
		for (int32 i = 0; i < BranchKeys.Num(); i++)
		{
			SmartDialogue->RemoveHideBranchByString(BranchKeys[i], BranchNameString);
			SmartDialogue->RemoveShowBranchByString(BranchKeys[i], BranchNameString);
		}
		SmartDialogue->Branches.Remove(BranchName);

		SmartDialogue->OnBranchRemoved.Broadcast(BranchName);

		return true;
	}
	return false;
}

bool UEditorDataHelper::RemoveVarOperation(FSmartDialogueEditor* SmartDialogueEditor, FName BranchName, const int32 Index)
{
	if (SmartDialogueEditor)
	{
		USmartDialogue* SmartDialogue = SmartDialogueEditor->GetDialogue();
		FScopedTransaction Transaction(TEXT("Remove Var Operation"), LOCTEXT("RemoveVarOperation", "Remove Var Operation"), SmartDialogue);

		if (SmartDialogue->Branches.Contains(BranchName))
		{
			SmartDialogue->Modify();
			const auto L_BranchPtr = &SmartDialogue->Branches[BranchName];
			L_BranchPtr->Vars.RemoveAt(Index);
			return true;
		}
	}
	return false;
}

bool UEditorDataHelper::RemoveIfOperation(FSmartDialogueEditor* SmartDialogueEditor, FName BranchName, int32 Index)
{
	if (SmartDialogueEditor)
	{
		USmartDialogue* SmartDialogue = SmartDialogueEditor->GetDialogue();
		FScopedTransaction Transaction(TEXT("Remove If Operation"), LOCTEXT("RemoveIfOperation", "Remove If Operation"), SmartDialogue);

		if (SmartDialogue->Branches.Contains(BranchName))
		{
			SmartDialogue->Modify();
			const auto L_BranchPtr = &SmartDialogue->Branches[BranchName];
			L_BranchPtr->If.RemoveAt(Index);
			return true;
		}
	}
	return false;
}

void UEditorDataHelper::AddVarOperation(FSmartDialogueEditor* SmartDialogueEditor, FName BranchName,
	const FString& VarName, const FString& OperationString, int32 Value)
{
	if (SmartDialogueEditor)
	{
		USmartDialogue* SmartDialogue = SmartDialogueEditor->GetDialogue();
		FScopedTransaction Transaction(TEXT("Add Var Operation"), LOCTEXT("AddVarOperation", "Add Var Operation"), SmartDialogue);

		if (SmartDialogue->Branches.Contains(BranchName))
		{
			const auto L_BranchPtr = &SmartDialogue->Branches[BranchName];

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

			SmartDialogue->Modify();
			L_BranchPtr->Vars.Add(NewItem);
		}
	}
}

void UEditorDataHelper::AddIfOperation(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	const FString& VarName, const FString& OperationString, int32 Value)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add If Operation"), LOCTEXT("AddIfOperation", "Add If Operation"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->AddIfOperation(BranchName, VarName, OperationString, Value);
	}
}

void UEditorDataHelper::AddHideBranchElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	const FString& Value)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add Hide Branch Element"), LOCTEXT("AddHideBranchElement", "Add Hide Branch Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->AddHideBranchElement(BranchName, Value);
	}
}

void UEditorDataHelper::RemoveHideBranchElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Hide Branch"), LOCTEXT("RemoveHideBranch", "Remove Hide Branch"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->RemoveHideBranchElement(BranchName, Index);
	}
}

void UEditorDataHelper::UpdateHideBranchElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	int32 Index, const FString& NewValue)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Update Hide Branch"), LOCTEXT("UpdateHideBranch", "Update Hide Branch"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->UpdateHideBranchElement(BranchName, Index, NewValue);
	}
}

void UEditorDataHelper::RemoveHideBranchByString(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	const FString& String)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Hide Branch"), LOCTEXT("RemoveHideBranch", "Remove Hide Branch"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->RemoveHideBranchByString(BranchName, String);
	}
}

void UEditorDataHelper::AddShowBranchElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	const FString& Value)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add Show Branch Element"), LOCTEXT("AddShowBranchElement", "Add Show Branch Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->AddShowBranchElement(BranchName, Value);
	}
}

void UEditorDataHelper::RemoveShowBranchElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Show Branch Element"), LOCTEXT("RemoveShowBranchElement", "Remove Show Branch Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->RemoveShowBranchElement(BranchName, Index);
	}
}

void UEditorDataHelper::UpdateShowBranchElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	int32 Index, const FString& NewValue)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Update Show Branch Element"), LOCTEXT("UpdateShowBranchElement", "Update Show Branch Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->UpdateShowBranchElement(BranchName, Index, NewValue);
	}
}

void UEditorDataHelper::RemoveShowBranchByString(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	const FString& String)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Show Branch By String"), LOCTEXT("RemoveShowBranchByString", "Remove Show Branch By String"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->RemoveShowBranchByString(BranchName, String);
	}
}

void UEditorDataHelper::AddVarElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	const FSmartDialogueVars& NewVar)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add Var Element"), LOCTEXT("AddVarElement", "Add Var Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->AddVarElement(BranchName, NewVar);
	}
}

void UEditorDataHelper::RemoveVarElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove Var Element"), LOCTEXT("RemoveVarElement", "Remove Var Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->RemoveVarElement(BranchName, Index);
	}
}

void UEditorDataHelper::UpdateVarElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	int32 Index, const FSmartDialogueVars& Element)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Update Var Element"), LOCTEXT("UpdateVarElement", "Update Var Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->UpdateVarElement(BranchName, Index, Element);
	}
}

void UEditorDataHelper::AddIfElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName,
	const FIf& Element)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Add If Element"), LOCTEXT("AddIfElement", "Add If Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->AddIfElement(BranchName, Element);
	}
}

void UEditorDataHelper::UpdateIfElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, int32 Index,
	const FIf& Element)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Update If Element"), LOCTEXT("UpdateIfElement", "Update If Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->UpdateIfElement(BranchName, Index, Element);
	}
}

void UEditorDataHelper::RemoveIfElement(FSmartDialogueEditor* SmartDialogueEditor, const FName& BranchName, int32 Index)
{
	if (SmartDialogueEditor)
	{
		FScopedTransaction Transaction(TEXT("Remove If Element"), LOCTEXT("RemoveIfElement", "Remove If Element"), SmartDialogueEditor->GetDialogue());
		SmartDialogueEditor->GetDialogue()->RemoveIfElement(BranchName, Index);
	}
}

void UEditorDataHelper::UpdateBranchText(FSmartDialogueEditor* Editor, const FName& BranchName, const FText& NewText)
{
	if (Editor)
	{
		if (Editor->GetDialogue()->GetBranches().Contains(BranchName))
		{
			const FScopedTransaction Transaction(TEXT("Set Branch Text"), LOCTEXT("SetBranchText", "Set Branch Text"), Editor->GetDialogue());
			Editor->GetDialogue()->Modify();
			Editor->GetDialogue()->GetBranchPtr(BranchName)->Text = NewText;
		}
	}

}

#undef LOCTEXT_NAMESPACE
