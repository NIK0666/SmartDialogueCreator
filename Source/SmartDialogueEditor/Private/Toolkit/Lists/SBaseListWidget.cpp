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
				SNew(SButton)
				.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
				.ContentPadding(FMargin(0.5, 1, 0.5, 1))
				.OnClicked(this, &SBaseListWidget::OnAddButtonClicked)
				[
					SNew(SImage)
					.Image(FEditorStyle::GetBrush("Plus"))
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

TArray< TSharedPtr< FString > > SBaseListWidget::GetAllStrings()
{
	TArray< TSharedPtr< FString > > AllStrings;
	// Fill the AllStrings array with the desired values.
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
	TArray<TSharedPtr<FString>> AllStrings = GetAllStrings();
	TSharedPtr<SComboBox<TSharedPtr<FString>>> ComboBox;
	SAssignNew(ComboBox, SComboBox<TSharedPtr<FString>>)
	.OptionsSource(&AllStrings)
	.OnGenerateWidget(this, &SBaseListWidget::GenerateStringItemWidget)
	.OnSelectionChanged(this, &SBaseListWidget::OnComboBoxSelectionChanged)
	.Content()
	[
		SNew(STextBlock)
		.Text(NSLOCTEXT("Temp", "SelectItem", "Select an item"))
	];

	// Создаем контекстное меню для выпадающего списка.
	FSlateApplication::Get().PushMenu(
		SharedThis(this),
		FWidgetPath(),
		ComboBox.ToSharedRef(),
		FSlateApplication::Get().GetCursorPos(),
		FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu)
	);

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