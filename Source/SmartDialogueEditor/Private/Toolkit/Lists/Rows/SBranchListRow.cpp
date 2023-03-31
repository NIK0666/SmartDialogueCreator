// SBranchListRow.cpp


#include "SBranchListRow.h"
#include "EditorStyleSet.h"
#include "Toolkit/Components/SBranchComboBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"

void SBranchListRow::Construct(const FArguments& InArgs)
{
	SBaseListRow::Construct(SBaseListRow::FArguments()
		.Item(InArgs._Item)
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
	UE_LOG(LogTemp, Log, TEXT("SBranchListRow::RemoveItem"));
	return SBaseListRow::RemoveItem();
}