// SDialConfigWidget.cpp continued

#include "SDialConfigWidget.h"

#include "Helpers/EditorDataHelper.h"
#include "SmartDialogueSettings.h"
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
								.OnClicked(this, &SDialConfigWidget::OnAddCharacterButtonClicked)
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

void SDialConfigWidget::UpdateData(bool bCharacters, bool bGlobalVars, bool bLocalVars, bool bParameters)
{
	
	if (DialConfig)
	{
		if (bCharacters)
		{
			ScrollBoxContent->ClearChildren();
			for (const auto L_Character : DialConfig->GetCharacters())
			{
				AddCharacterRow(L_Character.Id, L_Character.Name);
			}			
		}
		if (bGlobalVars)
		{
			ScrollBoxGlobalVarsContent->ClearChildren();
			for (const auto L_Var : DialConfig->GetVariables())
			{
				AddGlobalVarRow(L_Var.Key, L_Var.Value, L_Var.Desc);
			}
		}
		if (bParameters)
		{
			ParametersScrollBoxContent->ClearChildren();
			for (const auto L_Var : DialConfig->GetCustomParameters())
			{
				AddParameterRow(L_Var.Key, L_Var.Desc);
			}
		}
	}

	if (bLocalVars && SmartDialogueEditor->GetDialogue())
	{
		ScrollBoxLocalVarsContent->ClearChildren();
		auto LocalVars = SmartDialogueEditor->GetDialogue()->GetVariables();
		for (auto Element : LocalVars)
		{
			AddLocalVarRow(Element.Key, Element.Value, Element.Desc);
		}		
	}
}


FReply SDialConfigWidget::OnAddCharacterButtonClicked()
{
	UEditorDataHelper::AddCharacter(SmartDialogueEditor, {});
	AddCharacterRow();
	return FReply::Handled();
}

FReply SDialConfigWidget::OnAddParameterClicked()
{
	UEditorDataHelper::AddCustomParameter(SmartDialogueEditor, {});
	AddParameterRow();
	return FReply::Handled();
}

void SDialConfigWidget::OnCharacterChanged(const FCharacterData& CharacterData, int32 RowIndex)
{
	UEditorDataHelper::UpdateCharacterByIndex(SmartDialogueEditor, RowIndex, CharacterData);
}

void SDialConfigWidget::OnCustomParameterChanged(const FCustomParameterData& CustomParameterData, int32 RowIndex)
{
	UEditorDataHelper::UpdateCustomParameterByIndex(SmartDialogueEditor, RowIndex, CustomParameterData);
}

void SDialConfigWidget::OnGlobalVarChanged(const FVariableData& VariableData, int32 RowIndex)
{
	UEditorDataHelper::UpdatePublicVariableByIndex(SmartDialogueEditor, RowIndex, VariableData);
}

void SDialConfigWidget::OnLocalVarChanged(const FVariableData& VariableData, int32 RowIndex)
{
	UEditorDataHelper::UpdateLocalVariableByIndex(SmartDialogueEditor, RowIndex, VariableData);
}

FReply SDialConfigWidget::OnAddPublicVarClicked()
{
	UEditorDataHelper::AddPublicVariable(SmartDialogueEditor, {});
	AddGlobalVarRow();

	return FReply::Handled();
}

FReply SDialConfigWidget::OnAddLocalVarClicked()
{
	FVariableData VariableData = {"", "", 0};
	UEditorDataHelper::AddNewLocalVariable(SmartDialogueEditor, VariableData);
	AddLocalVarRow(VariableData.Key, VariableData.Value, VariableData.Desc);

	return FReply::Handled();
}

void SDialConfigWidget::OnCharacterSelected(TSharedPtr<FString> NewSelection)
{
	DialConfig->SetHero(*NewSelection);
}

void SDialConfigWidget::AddCharacterRow(FString Id, FText Name)
{
	const int32 RowIndex = ScrollBoxContent->GetChildren()->Num();
	
	ScrollBoxContent->AddSlot()
	.AutoHeight()
	.Padding(2.0f)
	[
		SNew(SCharacterListRow)
		.Id(Id)
		.Name(Name)
		.OnChanged(this, &SDialConfigWidget::OnCharacterChanged, RowIndex)
		.OnDeleteButtonClicked(FSimpleDelegate::CreateLambda([this, RowIndex]()
		{
			UEditorDataHelper::RemoveCharacterByIndex(SmartDialogueEditor, RowIndex);
			UpdateData(true, false, false, false);
		}))
	];
}

void SDialConfigWidget::AddGlobalVarRow(const FString& Key, const int32& Value, const FString& Desc)
{
	const int32 RowIndex = ScrollBoxGlobalVarsContent->GetChildren()->Num();

	ScrollBoxGlobalVarsContent->AddSlot()
	.AutoHeight()
	.Padding(2.0f)
	[
		SNew(SDialVarListRow)
		.VarKey(Key)
		.VarValue(Value)
		.VarDesc(Desc)
		.OnChanged(this, &SDialConfigWidget::OnGlobalVarChanged, RowIndex)
		.OnDeleteButtonClicked(FSimpleDelegate::CreateLambda([this, RowIndex]()
		{
			UEditorDataHelper::RemovePublicVariableByIndex(SmartDialogueEditor, RowIndex);
			UpdateData(false, true, false, false);			
		}))
	];
}

void SDialConfigWidget::AddLocalVarRow(const FString& Key, const int32& Value, const FString& Desc)
{
	
	const int32 RowIndex = ScrollBoxLocalVarsContent->GetChildren()->Num();

	ScrollBoxLocalVarsContent->AddSlot()
	.AutoHeight()
	.Padding(2.0f)
	[
		SNew(SDialVarListRow)
		.VarKey(Key)
		.VarValue(Value)
		.VarDesc(Desc)
		.OnChanged(this, &SDialConfigWidget::OnLocalVarChanged, RowIndex)
		.OnDeleteButtonClicked(FSimpleDelegate::CreateLambda([this, RowIndex]()
		{
			UEditorDataHelper::RemoveLocalVariableByIndex(SmartDialogueEditor, RowIndex);
			UpdateData(false, false, true, false);			
		}))
	];
}

void SDialConfigWidget::AddParameterRow(const FString& Key, const FString& Desc)
{
	const int32 RowIndex = ParametersScrollBoxContent->GetChildren()->Num();
	
	ParametersScrollBoxContent->AddSlot()
		.AutoHeight()
		.Padding(2.0f)
		[
			SNew(SParameterListRow)
			.Parameter(Key)
			.Desc(Desc)
			.OnChanged(this, &SDialConfigWidget::OnCustomParameterChanged, RowIndex)
			.OnDeleteButtonClicked(FSimpleDelegate::CreateLambda([this, RowIndex]()
			{
				UEditorDataHelper::RemoveCustomParameterByIndex(SmartDialogueEditor, RowIndex);
				UpdateData(false, false, false, true);
			}))
		];
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