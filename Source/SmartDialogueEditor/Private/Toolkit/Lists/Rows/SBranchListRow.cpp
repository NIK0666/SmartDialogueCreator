// SBranchListRow.cpp


#include "SBranchListRow.h"
#include "EditorStyleSet.h"
#include "SmartDialogue.h"
#include "Toolkit/Components/SBranchComboBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"

void SBranchListRow::Construct(const FArguments& InArgs)
{
	SBaseListRow::Construct(SBaseListRow::FArguments()
		.Item(InArgs._Item)
		.OnRemoveItemRequested(InArgs._OnRemoveItemRequested)
		.Editor(InArgs._Editor));
	
	bIsShowed = InArgs._bIsShowed;
}

TSharedRef<SWidget> SBranchListRow::GetContent()
{
	
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SNew(SBranchComboBox)
			.SmartDialogueEditor(Editor)
			.OnItemSelected_Lambda([this](TSharedPtr<FString> NewSelection)
			{
				if (bIsShowed)
				{
					for (int32 i = 0; i < Editor->GetSelectedBranch()->Show.Num(); i++)
					{
						if (Editor->GetSelectedBranch()->Show[i] == Item.Name)
						{
							Editor->GetSelectedBranch()->Show[i] = *NewSelection;
							Item.Name = *NewSelection;
							break;
						}
					}
				}
				else
				{
					for (int32 i = 0; i < Editor->GetSelectedBranch()->Hide.Num(); i++)
					{
						if (Editor->GetSelectedBranch()->Hide[i] == Item.Name)
						{
							Editor->GetSelectedBranch()->Hide[i] = *NewSelection;
							Item.Name = *NewSelection;
							break;
						}
					}
				}
			})
			.DefaultText(Item.Name)
		];
}

FReply SBranchListRow::RemoveItem()
{
	if (Editor->GetDialogue())
	{
		if (bIsShowed)
		{
			Editor->GetDialogue()->RemoveShowBranch(Editor->GetSelectedBranchName(), Item.Name);
		}
		else
		{
			Editor->GetDialogue()->RemoveHideBranch(Editor->GetSelectedBranchName(), Item.Name);
		}		
	}
	return SBaseListRow::RemoveItem();
}