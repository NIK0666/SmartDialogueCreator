// SBaseListWidget.cpp


#include "SBaseListWidget.h"

#include "EditorStyleSet.h"
#include "Rows/SBaseListRow.h"
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
			.VAlign(VAlign_Center)
			.Padding(4.f, 2.f)
			[
				SNew(STextBlock)
				.Text(Title)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(4.f, 2.f)
			[
				SAssignNew(ContextMenuAnchor, SMenuAnchor)
				.OnGetMenuContent(this, &SBaseListWidget::CreateMenuContent)
				[
					SNew(SButton)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.ButtonStyle(FAppStyle::Get(), "FlatButton.Success")
					.OnClicked(this, &SBaseListWidget::OnAddButtonClicked)
					[
						SNew(SImage)
						.Image(FAppStyle::GetBrush("Plus"))
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
}

void SBaseListWidget::UpdateData(const TArray<FListItemData>& NewData)
{
	Data = NewData;
	ListContainer->ClearChildren();

	for (int32 Index = 0; Index < Data.Num(); Index++)
	{
		const FListItemData& Item = Data[Index];

		ListContainer->AddSlot()
		[
			GetItemContent({Item})
		];
	}
}

TArray<TSharedPtr<FString>> SBaseListWidget::GetAllStrings()
{
	return {};
}

void SBaseListWidget::OnSelected(const FListItemData& SelectedItem)
{
	// Handle the selection of an item from the combo box.
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
	return SNew(SBaseListRow)
		.Item(Item)
		.Editor(Editor);
}

TSharedRef<SWidget> SBaseListWidget::CreateMenuContent()
{
	TArray<TSharedPtr<FString>> AllStrings = GetAllStrings();

	TSharedRef<SVerticalBox> MenuContent = SNew(SVerticalBox);
	for (const TSharedPtr<FString>& StringPtr : AllStrings)
	{
		MenuContent->AddSlot()
		[
			SNew(SButton)
			.Text(FText::FromString(*StringPtr))
			.OnClicked(this, FOnClicked::TMethodPtr<SBaseListWidget, FString>(&SBaseListWidget::OnContextMenuItemClicked), *StringPtr)
		];
	}

	return MenuContent;
}

FReply SBaseListWidget::OnContextMenuItemClicked(const FString& Item)
{
	// Обработка выбора элемента меню
	UE_LOG(LogTemp, Log, TEXT("Выбран элемент меню: %s"), *Item);

	// Закройте контекстное меню после выбора элемента
	ContextMenuAnchor->SetIsOpen(false);

	return FReply::Handled();
}