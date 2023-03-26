// SVerticalListWidget.cpp


#include "SVerticalListWidget.h"

#include "EditorStyleSet.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"

void SVerticalListWidget::Construct(const FArguments& InArgs)
{
	Title = InArgs._Title;

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
			.FillWidth(1.0f)
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
				.OnClicked(this, &SVerticalListWidget::OnAddButtonClicked)
				[
					SNew(SImage)
					.Image(FEditorStyle::GetBrush("Plus"))
				]
			]
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(ListContainer, SVerticalBox)
		]
	];

	UpdateData(/*TArray<FString>()*/{TEXT("Item 1"), TEXT("Item 2")}); // Initialize the list with an empty array.
}

void SVerticalListWidget::UpdateData(const TArray<FString>& NewData)
{
	Data = NewData;
	ListContainer->ClearChildren();

	for (int32 Index = 0; Index < Data.Num(); ++Index)
	{
		const FString& Item = Data[Index];
		
		ListContainer->AddSlot()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromString(Item))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.ButtonStyle(FEditorStyle::Get(), "FlatButton.Primary")
				.ContentPadding(FMargin(0.5f, 1, 0.5f, 1))
				.OnClicked(this, &SVerticalListWidget::OnChangeButtonClicked)
				[
					SNew(SImage)
					.Image(FEditorStyle::GetBrush("Icons.Edit"))
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.ButtonStyle(FEditorStyle::Get(), "FlatButton.Danger")
				.ContentPadding(FMargin(0.5f, 1, 0.5f, 1))
				.OnClicked_Lambda([this, Index](){ return this->OnRemoveButtonClicked(Index); })
				[
					SNew(SImage)
					.Image(FEditorStyle::GetBrush("Cross"))
				]
			]
		];
	}
}

TArray< TSharedPtr< FString > > SVerticalListWidget::GetAllStrings()
{
	TArray< TSharedPtr< FString > > AllStrings;
	// Fill the AllStrings array with the desired values.
	return AllStrings;
}

void SVerticalListWidget::OnSelected(const FString& SelectedItem)
{
	// Handle the selection of an item from the combo box.
}

FReply SVerticalListWidget::OnChangeButtonClicked()
{
	ShowSelectionMenu();
	return FReply::Handled();
}

FReply SVerticalListWidget::OnRemoveButtonClicked(int32 IndexToRemove)
{
	if (IndexToRemove != INDEX_NONE)
	{
		Data.RemoveAt(IndexToRemove);
		UpdateData(Data);
	}
	return FReply::Handled();
}

FReply SVerticalListWidget::OnAddButtonClicked()
{
	TArray<TSharedPtr<FString>> AllStrings = GetAllStrings();
	TSharedPtr<SComboBox<TSharedPtr<FString>>> ComboBox;
	SAssignNew(ComboBox, SComboBox<TSharedPtr<FString>>)
	.OptionsSource(&AllStrings)
	.OnGenerateWidget(this, &SVerticalListWidget::GenerateStringItemWidget)
	.OnSelectionChanged(this, &SVerticalListWidget::OnComboBoxSelectionChanged)
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

TSharedRef<SWidget> SVerticalListWidget::GenerateStringItemWidget(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock).Text(FText::FromString(*InItem));
}

void SVerticalListWidget::OnComboBoxSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	if (NewSelection.IsValid())
	{
		OnSelected(*NewSelection);
	}
}

void SVerticalListWidget::ShowSelectionMenu()
{
	TArray<TSharedPtr<FString>> AllStrings = GetAllStrings();
	TSharedPtr<SComboBox<TSharedPtr<FString>>> ComboBox;
	SAssignNew(ComboBox, SComboBox<TSharedPtr<FString>>)
	.OptionsSource(&AllStrings)
	.OnGenerateWidget(this, &SVerticalListWidget::GenerateStringItemWidget)
	.OnSelectionChanged(this, &SVerticalListWidget::OnComboBoxSelectionChanged)
	.Content()
	[
		SNew(STextBlock)
		.Text(NSLOCTEXT("Temp", "SelectItem", "Select an item"))
	];

	FSlateApplication::Get().PushMenu(
		SharedThis(this),
		FWidgetPath(),
		ComboBox.ToSharedRef(),
		FSlateApplication::Get().GetCursorPos(),
		FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu)
	);
}