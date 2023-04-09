// SDialConfigWidget.cpp continued

#include "SDialConfigWidget.h"
#include "SmartDialogueCore/Private/SmartDialogueSettings.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorStyleSet.h"
#include "SmartDialConfig.h"
#include "SmartDialogue.h"
#include "Components/SCharacterComboBox.h"
#include "Lists/Rows/SCharacterListRow.h"
#include "Lists/Rows/SDialVarListRow.h"
#include "Lists/Rows/SParameterListRow.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

void SDialConfigWidget::Construct(const FArguments& InArgs)
{
	SmartDialogueEditor = InArgs._SmartDialogueEditor;
	DialConfig = SmartDialogueEditor->GetDialogueConfig();
	
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
								.ButtonStyle(FAppStyle::Get(), "FlatButton.Success")
								.ForegroundColor(FSlateColor::UseForeground())
								.OnClicked(this, &SDialConfigWidget::OnAddButtonClicked)
								[
									SNew(SImage)
									.Image(FAppStyle::Get().GetBrush("Icons.Plus"))
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
								SAssignNew(CharacterComboBox, SCharacterComboBox)
								.SmartDialogueEditor(SmartDialogueEditor)
								.OnItemSelected(this, &SDialConfigWidget::OnCharacterSelected)
								.DefaultText(DialConfig->GetHero())								
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
							.ButtonStyle(FAppStyle::Get(), "FlatButton.Success")
							.ForegroundColor(FSlateColor::UseForeground())
							.OnClicked(this, &SDialConfigWidget::OnAddPublicVarClicked)
							.ToolTipText(FText::FromString("Add public var"))
							[
								SNew(SImage)
								.Image(FAppStyle::Get().GetBrush("Icons.Plus"))
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
					// Third tab (Local Vars) content
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SButton)
							.ButtonStyle(FAppStyle::Get(), "FlatButton.Success")
							.ForegroundColor(FSlateColor::UseForeground())
							.OnClicked(this, &SDialConfigWidget::OnAddLocalVarClicked)
							.ToolTipText(FText::FromString("Add local var"))
							[
								SNew(SImage)
								.Image(FAppStyle::Get().GetBrush("Icons.Plus"))
							]
						]
					]
					+ SVerticalBox::Slot()
					.FillHeight(1.0f)
					[
						SNew(SScrollBox)
						+ SScrollBox::Slot()
						[
							SAssignNew(ScrollBoxLocalVarsContent, SVerticalBox)
						]
					]
				]
				+ SWidgetSwitcher::Slot()
				[
					// (Parameters) content
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
								.ButtonStyle(FAppStyle::Get(), "FlatButton.Success")
								.ForegroundColor(FSlateColor::UseForeground())
								.OnClicked(this, &SDialConfigWidget::OnAddParameterClicked)
								[
									SNew(SImage)
									.Image(FAppStyle::Get().GetBrush("Icons.Plus"))
								]
							]
						]
						+ SVerticalBox::Slot()
						.FillHeight(1.0f)
						[
							SNew(SScrollBox)
							+ SScrollBox::Slot()
							[
								SAssignNew(ParametersScrollBoxContent, SVerticalBox)
							]
						]
					]
				]
			]
		];
	
	UpdateButtonStyles();

	UpdateData();
}

void SDialConfigWidget::UpdateData()
{
	ScrollBoxContent->ClearChildren();
	ScrollBoxGlobalVarsContent->ClearChildren();
	ScrollBoxLocalVarsContent->ClearChildren();
	ParametersScrollBoxContent->ClearChildren();
	
	if (DialConfig)
	{
		for (const auto L_Character : DialConfig->GetCharacters())
		{
			AddCharacterRow(L_Character.Id, L_Character.Name);
		}
		for (const auto L_Var : DialConfig->GetVariables())
		{
			AddGlobalVarRow(L_Var.Key, L_Var.Value, L_Var.Desc);
		}
		for (const auto L_Var : DialConfig->GetCustomParameters())
		{
			AddParameterRow(L_Var.Key, L_Var.Desc);
		}
	}

	if (SmartDialogueEditor->GetDialogue())
	{
		auto LocalVars = SmartDialogueEditor->GetDialogue()->GetVariables();
		for (auto Element : LocalVars)
		{
			AddLocalVarRow(Element.Key, Element.Value, Element.Desc);
		}		
	}
}


FReply SDialConfigWidget::OnAddButtonClicked()
{
	// Add a new character with default values
	AddCharacterRow();
	return FReply::Handled();
}

FReply SDialConfigWidget::OnAddParameterClicked()
{
	AddParameterRow();
	return FReply::Handled();
}

void SDialConfigWidget::OnCharacterSelected(TSharedPtr<FString> NewSelection)
{
	DialConfig->SetHero(*NewSelection);
}

void SDialConfigWidget::OnDeleteButtonClicked(TSharedPtr<FString> CharacterId)
{
	// Remove character with the specified ID
}

void SDialConfigWidget::AddCharacterRow(FString Id, FText Name)
{
	TSharedPtr<FString> SharedId = MakeShared<FString>(Id);

	ScrollBoxContent->AddSlot()
	.AutoHeight()
	.Padding(2.0f)
	[
		SNew(SCharacterListRow)
		.Id(Id)
		.Name(Name)
		.OnDeleteButtonClicked(FSimpleDelegate::CreateLambda([this, SharedId]()
		{
			this->OnDeleteButtonClicked(SharedId);
		}))
	];
}

void SDialConfigWidget::AddGlobalVarRow(const FString& Key, const int32& Value, const FString& Desc)
{
	TSharedPtr<FString> SharedId = MakeShared<FString>(Key);

	ScrollBoxGlobalVarsContent->AddSlot()
	.AutoHeight()
	.Padding(2.0f)
	[
		SNew(SDialVarListRow)
		.VarKey(Key)
		.VarValue(Value)
		.VarDesc(Desc)
		.OnDeleteButtonClicked(FSimpleDelegate::CreateLambda([this, SharedId]()
		{
			this->OnDeleteButtonClicked(SharedId);
		}))
	];
}

void SDialConfigWidget::AddLocalVarRow(const FString& Key, const int32& Value, const FString& Desc)
{
	TSharedPtr<FString> SharedId = MakeShared<FString>(Key);

	ScrollBoxLocalVarsContent->AddSlot()
	.AutoHeight()
	.Padding(2.0f)
	[
		SNew(SDialVarListRow)
		.VarKey(Key)
		.VarValue(Value)
		.VarDesc(Desc)
		.OnDeleteButtonClicked(FSimpleDelegate::CreateLambda([this, SharedId]()
		{
			// this->OnDeleteButtonClicked(SharedId);
		}))
	];
}

void SDialConfigWidget::AddParameterRow(const FString& Key, const FString& Desc)
{
	TSharedPtr<FString> SharedId = MakeShared<FString>(Key);
	ParametersScrollBoxContent->AddSlot()
		.AutoHeight()
		.Padding(2.0f)
		[
			SNew(SParameterListRow)
			.Parameter(Key)
			.Desc(Desc)
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
		SNew(SDialVarListRow)
			.VarKey(*NewVarId.Get())
	];

	return FReply::Handled();
}

FReply SDialConfigWidget::OnAddLocalVarClicked()
{
	// Создайте новый идентификатор переменной и добавьте его в список
	TSharedPtr<FString> NewVarId = MakeShareable(new FString(FString::Printf(TEXT("LocalVar%d"), LocalVarCounter++)));
	LocalVarIds.Add(NewVarId);

	// Добавьте новую строку в список локальных переменных
	ScrollBoxLocalVarsContent->AddSlot()
	[
		SNew(SDialVarListRow)
			.VarKey(*NewVarId.Get())
	];

	return FReply::Handled();
}

void SDialConfigWidget::OnLocalVarDeleted(TSharedPtr<FString> VarId)
{
	
}

void SDialConfigWidget::OnPublicVarDeleted(TSharedPtr<FString> VarId)
{
	
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