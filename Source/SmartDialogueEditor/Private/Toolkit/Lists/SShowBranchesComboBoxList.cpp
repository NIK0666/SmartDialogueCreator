// 


#include "SShowBranchesComboBoxList.h"
#include "SmartDialogue.h"


void SShowBranchesComboBoxList::Construct(const FArguments& InArgs)
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
                    .OnClicked(this, &SShowBranchesComboBoxList::OnAddButtonClicked)
			]
		]
		+ SVerticalBox::Slot()
		[
			SAssignNew(ListBox, SVerticalBox)
		]
	];

	RefreshList();
}

void SShowBranchesComboBoxList::UpdateInitialStrings(const TArray<FString>& NewInitialStrings)
{
	InitialStrings = NewInitialStrings;
	RefreshList();
}

void SShowBranchesComboBoxList::OnComboBoxOpeningHandler()
{
	FSlateApplication::Get().SetKeyboardFocus(ListBox);

	GetOptions();
}

void SShowBranchesComboBoxList::RefreshList()
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
				.OnSelectionChanged(this, &SShowBranchesComboBoxList::OnSelectionChanged, Index)
				.InitiallySelectedItem(SelectedItem)
				.OnGenerateWidget(this, &SShowBranchesComboBoxList::MakeWidgetForOption)
				.OnComboBoxOpening(this, &SShowBranchesComboBoxList::OnComboBoxOpeningHandler)
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
				.OnClicked(this, &SShowBranchesComboBoxList::OnDeleteButtonClicked, Index)
			]
		];
	}
}

void SShowBranchesComboBoxList::GetOptions()
{
	Options = Editor->GetAllBranchesList();
}

TSharedRef<SWidget> SShowBranchesComboBoxList::MakeWidgetForOption(TSharedPtr<FString> InOption)
{
	return SNew(STextBlock).Text(FText::FromString(*InOption));
}

FReply SShowBranchesComboBoxList::OnAddButtonClicked()
{
	
	InitialStrings.Add(*Options[0]);
	RefreshList();
	Editor->GetDialogue()->AddShowBranchElement(Editor->GetSelectedBranchName(), *Options[0]);

	return FReply::Handled();
}

void SShowBranchesComboBoxList::OnSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo, int32 Index)
{
	if (NewValue.IsValid())
	{
		InitialStrings[Index] = *NewValue.Get();
		Editor->GetDialogue()->UpdateShowBranchElement(Editor->GetSelectedBranchName(),Index, *NewValue.Get());
	}
}

FReply SShowBranchesComboBoxList::OnDeleteButtonClicked(int32 Index)
{
	// Удалите выбранный элемент из массива InitialStrings и обновите список
	if (InitialStrings.IsValidIndex(Index))
	{
		InitialStrings.RemoveAt(Index);
		RefreshList();
		Editor->GetDialogue()->RemoveShowBranchElement(Editor->GetSelectedBranchName(),Index);
	}

	return FReply::Handled();
}
