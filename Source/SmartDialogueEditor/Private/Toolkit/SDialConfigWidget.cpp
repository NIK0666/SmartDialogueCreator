// SDialConfigWidget.cpp continued

#include "SDialConfigWidget.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorStyleSet.h"
#include "Lists/Items/SCharacterRowWidget.h"

void SDialConfigWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	.VAlign(VAlign_Fill)
	.HAlign(HAlign_Fill)
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
				.ForegroundColor(FSlateColor::UseForeground())
				.OnClicked(this, &SDialConfigWidget::OnAddButtonClicked)
				[
					SNew(SImage)
					.Image(FEditorStyle::Get().GetBrush("Icons.Plus"))
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Hero:"))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(CharacterComboBox, SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&CharacterList)
				.OnSelectionChanged(this, &SDialConfigWidget::OnCharacterSelected)
				.Content()
				[
					SNew(STextBlock)
					.Text_Lambda([this]()
					{
						TSharedPtr<FString> SelectedItem = CharacterComboBox->GetSelectedItem();
						if (SelectedItem.IsValid())
						{
							return FText::FromString(*SelectedItem);
						}
						else
						{
							return FText::FromString(TEXT("[character]"));
						}
					})
				]
			]
		]
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
			[
				SAssignNew(ScrollBoxContent, SVerticalBox)
			]
		]
	];

	// Add a sample character as an example
	AddCharacterRow("1", "John Doe");
}

FReply SDialConfigWidget::OnAddButtonClicked()
{
	// Add a new character with default values
	AddCharacterRow();
	return FReply::Handled();
}

void SDialConfigWidget::OnCharacterSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	// Handle character selection change
}

void SDialConfigWidget::OnDeleteButtonClicked(TSharedPtr<FString> CharacterId)
{
	// Remove character with the specified ID
}

void SDialConfigWidget::AddCharacterRow(FString Id, FString Name)
{
	TSharedPtr<FString> SharedId = MakeShared<FString>(Id);

	ScrollBoxContent->AddSlot()
	.AutoHeight()
	.Padding(2.0f)
	[
		SNew(SCharacterRowWidget)
		.Id(Id)
		.Name(Name)
		.OnDeleteButtonClicked(FSimpleDelegate::CreateLambda([this, SharedId]()
		{
			this->OnDeleteButtonClicked(SharedId);
		}))
	];
}