// SDialConfigWidget.cpp continued

#include "SDialConfigWidget.h"
#include "SmartDialogueCore/Private/SmartDialogueSettings.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorStyleSet.h"
#include "SmartDialConfig.h"
#include "Lists/Rows/SCharacterListRow.h"
#include "Lists/Rows/SDialVarListRow.h"
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
									SAssignNew(CharacterTextBlock, STextBlock)
									.Text(FText::GetEmpty())
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
							.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
							.ForegroundColor(FSlateColor::UseForeground())
							.OnClicked(this, &SDialConfigWidget::OnAddLocalVarClicked)
							.ToolTipText(FText::FromString("Add local var"))
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
							SAssignNew(ScrollBoxLocalVarsContent, SVerticalBox)
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
						.Text(FText::FromString("Parameters content will be here."))
					]
				]
			]
		];
	
	UpdateButtonStyles();

	UpdateData();
}

void SDialConfigWidget::UpdateData()
{
	const USmartDialogueSettings* Settings = GetDefault<USmartDialogueSettings>();
	FSoftObjectPath DialoguesConfigAssetPath = Settings->DialoguesConfigAsset;

	if (DialoguesConfigAssetPath.IsValid())
	{
		FStringAssetReference AssetRef(DialoguesConfigAssetPath);
		UObject* LoadedAsset = AssetRef.TryLoad();

		DialConfig = Cast<USmartDialConfig>(LoadedAsset);
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

			for (auto Element : CharacterList)
			{
				if (Element.Get()->Equals(DialConfig->GetHero()))
				{
					CharacterComboBox->SetSelectedItem(Element);
					break;
				}		
			}
			
		}
	}
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
		SNew(SCharacterListRow)
		.Id(Id)
		.Name(Name)
		.OnDeleteButtonClicked(FSimpleDelegate::CreateLambda([this, SharedId]()
		{
			this->OnDeleteButtonClicked(SharedId);
		}))
	];
}

void SDialConfigWidget::AddGlobalVarRow(const FString& Key, const FString& Value, const FString& Desc)
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