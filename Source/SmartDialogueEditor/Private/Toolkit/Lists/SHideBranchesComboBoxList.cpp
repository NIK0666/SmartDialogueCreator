// 


#include "SHideBranchesComboBoxList.h"

#include "SmartDialogue.h"

void SHideBranchesComboBoxList::Construct(const FArguments& InArgs)
{
	InitialStrings = InArgs._InitialStrings;
	Editor = InArgs._Editor;
	const FText Title = InArgs._Title;
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(Title)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
                    .Text(FText::FromString("Add"))
                    .OnClicked(this, &SHideBranchesComboBoxList::OnAddButtonClicked)
			]
		]
		+ SVerticalBox::Slot()
		[
			SAssignNew(ListBox, SVerticalBox)
		]
	];

	RefreshList();
}

void SHideBranchesComboBoxList::UpdateInitialStrings(const TArray<FString>& NewInitialStrings)
{
	InitialStrings = NewInitialStrings;
	RefreshList();
}

void SHideBranchesComboBoxList::OnComboBoxOpeningHandler()
{
	FSlateApplication::Get().SetKeyboardFocus(ListBox);

	GetOptions();
}

void SHideBranchesComboBoxList::RefreshList()
{
	ListBox->ClearChildren();

	GetOptions();

	for (int32 Index = 0; Index < InitialStrings.Num(); ++Index)
	{
		TSharedPtr<FString> SelectedItem = Options[0];

		for (auto Element : Options)
		{
			if (InitialStrings[Index] == *Element.Get())
			{
				SelectedItem = Element;
				break;
			}
		}
		
		ListBox->AddSlot()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&Options)
				.OnSelectionChanged(this, &SHideBranchesComboBoxList::OnSelectionChanged, Index)
				.InitiallySelectedItem(SelectedItem)
				.OnGenerateWidget(this, &SHideBranchesComboBoxList::MakeWidgetForOption)
				.OnComboBoxOpening(this, &SHideBranchesComboBoxList::OnComboBoxOpeningHandler)
				.Content()
				[
					SNew(STextBlock)
					.Text_Lambda([this, Index] { return FText::FromString(*InitialStrings[Index]); })
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Text(FText::FromString("X"))
				.OnClicked(this, &SHideBranchesComboBoxList::OnDeleteButtonClicked, Index)
			]
		];
	}
}

void SHideBranchesComboBoxList::GetOptions()
{
	Options = Editor->GetAllBranchesList();
}

TSharedRef<SWidget> SHideBranchesComboBoxList::MakeWidgetForOption(TSharedPtr<FString> InOption)
{
	return SNew(STextBlock).Text(FText::FromString(*InOption));
}

FReply SHideBranchesComboBoxList::OnAddButtonClicked()
{
	
	InitialStrings.Add(*Options[0]);
	RefreshList();
	Editor->GetDialogue()->AddHideBranchElement(Editor->GetSelectedBranchName(), InitialStrings.Num() - 1, *Options[0]);

	return FReply::Handled();
}

void SHideBranchesComboBoxList::OnSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo, int32 Index)
{
	if (NewValue.IsValid())
	{
		InitialStrings[Index] = *NewValue.Get();
		Editor->GetDialogue()->UpdateHideBranchElement(Editor->GetSelectedBranchName(),Index, *NewValue.Get());
	}
}

FReply SHideBranchesComboBoxList::OnDeleteButtonClicked(int32 Index)
{
	// Удалите выбранный элемент из массива InitialStrings и обновите список
	if (InitialStrings.IsValidIndex(Index))
	{
		InitialStrings.RemoveAt(Index);
		RefreshList();
		Editor->GetDialogue()->RemoveHideBranchElement(Editor->GetSelectedBranchName(),Index);
	}

	return FReply::Handled();
}
