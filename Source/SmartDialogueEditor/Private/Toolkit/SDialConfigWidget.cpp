// SDialConfigWidget.cpp continued

#include "SDialConfigWidget.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorStyleSet.h"
#include "Lists/Items/SCharacterRowWidget.h"
#include "Lists/Items/SGlobalVarRow.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

void SDialConfigWidget::Construct(const FArguments& InArgs)
{
	AddTabButton("Characters");
	AddTabButton("Global Vars");
	AddTabButton("Local Vars");
	AddTabButton("Parameters");

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
					TabButtons[0].ToSharedRef()
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					TabButtons[1].ToSharedRef()
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					TabButtons[2].ToSharedRef()
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					TabButtons[3].ToSharedRef()
				]
			]
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SAssignNew(WidgetSwitcher, SWidgetSwitcher)
				+ SWidgetSwitcher::Slot()
				[
					// First tab (Characters) content
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
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
					]
				]
				+ SWidgetSwitcher::Slot()
				[
					// Second tab (Global Vars) content
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
							.OnClicked(this, &SDialConfigWidget::OnAddPublicVarClicked)
							.ToolTipText(FText::FromString("Add public var"))
							[
								SNew(SImage)
								.Image(FEditorStyle::Get().GetBrush("Icons.Plus"))
							]
						]
					]
					+ SVerticalBox::Slot()
					.FillHeight(1.0f)
					[
						SNew(SScrollBox)
						+ SScrollBox::Slot()
						[
							SAssignNew(ScrollBoxGlobalVarsContent, SVerticalBox)
						]
					]
				]

				+ SWidgetSwitcher::Slot()
				[
					// Fourth tab (Parameters) content
					SNew(SBox)
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Local Vars content will be here."))
					]
				]
				+ SWidgetSwitcher::Slot()
				[
					// Fourth tab (Parameters) content
					SNew(SBox)
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Parameters content will be here."))
					]
				]
			]
		];

	AddCharacterRow("1", "John Doe");
	UpdateButtonStyles();
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

FReply SDialConfigWidget::OnAddPublicVarClicked()
{
	// Создайте новый идентификатор переменной и добавьте его в список
	TSharedPtr<FString> NewVarId = MakeShareable(new FString(FString::Printf(TEXT("Var%d"), PublicVarCounter++)));
	PublicVarIds.Add(NewVarId);

	// Добавьте новую строку в список глобальных переменных
	ScrollBoxGlobalVarsContent->AddSlot()
	[
		SNew(SGlobalVarRow)
			.VarId(NewVarId)
			.ParentWidget(SharedThis(this))
	];

	return FReply::Handled();
}

void SDialConfigWidget::OnPublicVarDeleted(TSharedPtr<FString> VarId)
{
	if (!VarId.IsValid())
	{
		return;
	}

	// // Удалите переменную из списка
	// PublicVarIds.Remove(VarId);
	//
	// // Удалите соответствующую строку из контента скроллбокса
	// TSharedPtr<SWidget> RowToRemove;
	// for (const TSharedRef<SWidget>& Child : ScrollBoxGlobalVarsContent->GetChildren())
	// {
	// 	TSharedPtr<SGlobalVarRow> GlobalVarRow = StaticCastSharedRef<SGlobalVarRow>(Child);
	// 	if (GlobalVarRow->GetVarId() == VarId)
	// 	{
	// 		RowToRemove = GlobalVarRow;
	// 		break;
	// 	}
	// }
	//
	// if (RowToRemove.IsValid())
	// {
	// 	ScrollBoxGlobalVarsContent->RemoveSlot(RowToRemove.ToSharedRef());
	// }
}

FReply SDialConfigWidget::OnTabButtonClicked(int32 TabIndex)
{
	ActiveTabIndex = TabIndex;
	WidgetSwitcher->SetActiveWidgetIndex(ActiveTabIndex);

	// Обновление стилей кнопок
	UpdateButtonStyles();

	return FReply::Handled();
}

void SDialConfigWidget::UpdateButtonStyles()
{
	for (int32 i = 0; i < TabButtons.Num(); ++i)
	{
		TSharedPtr<SButton> Button = TabButtons[i];
		if (Button.IsValid())
		{
			if (ActiveTabIndex == i)
			{
				Button->SetEnabled(false);
			}
			else
			{
				Button->SetEnabled(true);
			}
		}
	}
}

void SDialConfigWidget::AddTabButton(const FString& ButtonText)
{
	int32 ButtonIndex = TabButtons.Num();
	TSharedRef<SButton> Button = SNew(SButton)
		.Text(FText::FromString(ButtonText))
		.OnClicked_Lambda([this, ButtonIndex](){
			return OnTabButtonClicked(ButtonIndex);
	    });

	TabButtons.Add(Button);
}
