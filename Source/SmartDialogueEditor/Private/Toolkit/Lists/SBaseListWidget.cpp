// SBaseListWidget.cpp


#include "SBaseListWidget.h"

#include "EditorStyleSet.h"
#include "SBaseListItemWidget.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"

void SBaseListWidget::Construct(const FArguments& InArgs)
{
	Title = InArgs._Title;
	Editor = InArgs._Editor;

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Top)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(Title)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(ContextMenuAnchor, SMenuAnchor)
				.OnGetMenuContent(this, &SBaseListWidget::CreateMenuContent)
				[
					SNew(SButton)
					.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
					.ContentPadding(FMargin(0.5, 1, 0.5, 1))
					.OnClicked(this, &SBaseListWidget::OnAddButtonClicked)
					[
						SNew(SImage)
						.Image(FEditorStyle::GetBrush("Plus"))
					]
				]
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(ListContainer, SVerticalBox)
		]
	];

	UpdateData({{"Test"}}); // Initialize the list with an empty array.
}

void SBaseListWidget::UpdateData(const TArray<FListItemData>& NewData)
{
	Data = NewData;
	ListContainer->ClearChildren();

	for (int32 Index = 0; Index < Data.Num(); ++Index)
	{
		const FListItemData& Item = Data[Index];

		ListContainer->AddSlot()
		[
			GetItemContent(Item)
		];
	}
}

TArray<FString> SBaseListWidget::GetAllStrings()
{
	TArray<FString> AllStrings;
	AllStrings.Add(TEXT("test1"));
	AllStrings.Add(TEXT("test2"));
	AllStrings.Add(TEXT("test___3"));

	return AllStrings;
}

void SBaseListWidget::OnSelected(const FListItemData& SelectedItem)
{
	// Handle the selection of an item from the combo box.
}

void SBaseListWidget::OnRemoveButtonClicked(const int32 IndexToRemove)
{
	if (IndexToRemove != INDEX_NONE)
	{
		Data.RemoveAt(IndexToRemove);
		UpdateData(Data);
	}
}

FReply SBaseListWidget::OnAddButtonClicked()
{
	if (ContextMenuAnchor->IsOpen())
	{
		ContextMenuAnchor->SetIsOpen(false);
	}
	else
	{
		ContextMenuAnchor->SetIsOpen(true);
	}

	return FReply::Handled();
}

TSharedRef<SWidget> SBaseListWidget::GenerateStringItemWidget(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock).Text(FText::FromString(*InItem));
}

void SBaseListWidget::OnComboBoxSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	if (NewSelection.IsValid())
	{
		FListItemData NewItem;
		NewItem.Name = *NewSelection;		
		OnSelected(NewItem);
	}
}

TSharedRef<SWidget> SBaseListWidget::GetItemContent(const FListItemData& Item)
{
	return SNew(SBaseListItemWidget)
		.Item(Item)
		.Editor(Editor);
}

TSharedRef<SWidget> SBaseListWidget::CreateMenuContent()
{
	TArray<FString> AllStrings = GetAllStrings();

	TSharedRef<SVerticalBox> MenuContent = SNew(SVerticalBox);
	for (const FString& String : AllStrings)
	{
		MenuContent->AddSlot()
		[
			SNew(SButton)
			.Text(FText::FromString(String))
			.OnClicked(this, FOnClicked::TMethodPtr<SBaseListWidget, FString>(&SBaseListWidget::OnMenuItemClicked), String)
		];
	}

	return MenuContent;
}

FReply SBaseListWidget::OnMenuItemClicked(const FString& Item)
{
	// Обработка выбора элемента меню
	UE_LOG(LogTemp, Log, TEXT("Выбран элемент меню: %s"), *Item);

	// Закройте контекстное меню после выбора элемента
	ContextMenuAnchor->SetIsOpen(false);

	return FReply::Handled();
}