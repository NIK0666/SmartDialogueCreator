// SBranchPropertiesWidget.cpp


#include "SBranchPropertiesWidget.h"

#include "EditorStyleSet.h"
#include "FSmartDialogueEditor.h"
#include "Components/SBranchComboBox.h"
#include "Lists/SBranchesListWidget.h"
#include "Lists/SOperationsListWidget.h"

#define LOCTEXT_NAMESPACE "SmartDialogueEditor"

void SBranchPropertiesWidget::Construct(const FArguments& InArgs) 
{
	SmartDialogueEditor = InArgs._SmartDialogueEditor;

	SmartDialogueEditor.Get()->OnBranchSelected.AddSP(this, &SBranchPropertiesWidget::OnBranchSelected);
	
	// AllBranchesList = SmartDialogueEditor.Get()->GetAllBranchesList();
	
	if (GetBranchDataPtr() != nullptr)
	{
		ContentWidget = GetContentWidget();
	}
	else
	{
		ContentWidget = SNullWidget::NullWidget;
	}

	ChildSlot
	[
		ContentWidget.ToSharedRef()
	];	
}

TSharedRef<SWidget> SBranchPropertiesWidget::GetContentWidget()
{
	if (!PropertiesContentWidget.IsValid())
	{
		PropertiesContentWidget =  SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(FMargin(0.0f, 4.0f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(4.f, 0.f)
			.VAlign(VAlign_Center)
			[
				SAssignNew(HiddenCheckBox, SCheckBox)
				.HAlign(HAlign_Left)
				.IsChecked(GetBranchDataPtr()->Hidden ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
				{					
					GetBranchDataPtr()->Hidden = NewState == ECheckBoxState::Checked;
					if (SmartDialogueEditor->GetSelectedBranch())
					{
						SmartDialogueEditor->GetSelectedBranch()->Hidden = NewState == ECheckBoxState::Checked;
					}					
				})
				.Content()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("HiddenLabel", "Hidden"))
				]
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(4.f, 0.f)
			.VAlign(VAlign_Center)
			[
				SAssignNew(HideSelfCheckBox, SCheckBox)
				.HAlign(HAlign_Left)
				.IsChecked(GetBranchDataPtr()->HideSelf ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
				{
					GetBranchDataPtr()->HideSelf = NewState == ECheckBoxState::Checked;
					if (SmartDialogueEditor->GetSelectedBranch())
					{
						SmartDialogueEditor->GetSelectedBranch()->HideSelf = NewState == ECheckBoxState::Checked;
					}
				})
				.Content()
				[
					SNew(STextBlock)
					.Margin(FMargin(4.f, 0.f))
					.Text(LOCTEXT("HideSelfLabel", "Hide Self"))
				]
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(4.f, 0.f)
			.VAlign(VAlign_Center)
			[
				SAssignNew(ClosedCheckBox, SCheckBox)
				.HAlign(HAlign_Left)
				.IsChecked(GetBranchDataPtr()->Closed ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
					{
						GetBranchDataPtr()->Closed = NewState == ECheckBoxState::Checked;
						if (SmartDialogueEditor->GetSelectedBranch())
						{
							SmartDialogueEditor->GetSelectedBranch()->Closed = NewState == ECheckBoxState::Checked;
						}
					})
				.Content()
				[
					SNew(STextBlock)
					.Margin(FMargin(4.f, 0.f))
					.Text(LOCTEXT("EndOfDialogueLabel", "End of Dialogue"))
				]
			]
			// "Show choice"
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(4.f, 0.f)
			.VAlign(VAlign_Center)
			[
				SAssignNew(ShowChoiceCheckBox, SCheckBox)
				.HAlign(HAlign_Left)
				.IsChecked(GetBranchDataPtr()->Choice ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
				{
					GetBranchDataPtr()->Choice = NewState == ECheckBoxState::Checked;
					if (SmartDialogueEditor->GetSelectedBranch())
					{
						SmartDialogueEditor->GetSelectedBranch()->Choice = NewState == ECheckBoxState::Checked;
					}
				})
				.Content()
				[
					SNew(STextBlock)
					.Margin(FMargin(4.f, 0.f))
					.Text(LOCTEXT("ShowChoiceLabel", "Show Choice"))
				]
			]

			// "Check as OR"
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(4.f, 0.f)
			.VAlign(VAlign_Center)
			[
				SAssignNew(CheckAsORCheckBox, SCheckBox)
				.HAlign(HAlign_Left)
				.IsChecked(GetBranchDataPtr()->OrCond ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
				{
					GetBranchDataPtr()->OrCond = NewState == ECheckBoxState::Checked;
					if (SmartDialogueEditor->GetSelectedBranch())
					{
						SmartDialogueEditor->GetSelectedBranch()->OrCond = NewState == ECheckBoxState::Checked;
					}
				})
				.Content()
				[
					SNew(STextBlock)
					.Margin(FMargin(4.f, 0.f))
					.Text(LOCTEXT("CheckAsORLabel", "Check as OR"))
				]
			]

			// Started Brahcn
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(StartBranchComboBox, SBranchComboBox)
				.SmartDialogueEditor(SmartDialogueEditor)
				.OnItemSelected_Lambda([this](TSharedPtr<FString> NewSelection)
				{
					GetBranchDataPtr()->ChangeStarted = *NewSelection;
				})
				.DefaultText(SmartDialogueEditor->GetSelectedBranch()->ChangeStarted)
			]			
			+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			[
				SNew(SBox)
				.HAlign(HAlign_Fill)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("EventLabel", "Event:"))
						.Margin(FMargin(4.f, 0.f))
					]

					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						SAssignNew(EventNameTextBox, SEditableTextBox)
						.Text(FText::FromString(GetBranchDataPtr()->Event.Name))
						.OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType) { GetBranchDataPtr()->Event.Name = NewText.ToString(); })
						.HintText(LOCTEXT("EventNameLabel", "Event name"))
					]

					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						SNew(SButton)
						.ButtonStyle(FEditorStyle::Get(), "HoverHintOnly")
						.OnClicked_Lambda([this]()
						{
							// SmartDialogueEditor->ShowEventSettings(GetBranchDataPtr()->Event);
							return FReply::Handled();
						})
						.ToolTipText(LOCTEXT("OpenEventSettingsLabel", "Open Event Settings"))
						.ContentPadding(2)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.HAlign(HAlign_Center)
							[
								SNew(SImage)
								.Image(FEditorStyle::GetBrush("Icons.Settings"))
							]
						]
					]
				]
			]
		]
		+ SVerticalBox::Slot()
		  .AutoHeight()
		  .VAlign(VAlign_Top)
		  .Padding(FMargin(0.0f, 4.0f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			  .FillWidth(1.0f)
			  .Padding(4.f, 0.f)
			  .VAlign(VAlign_Top)
			[
			SAssignNew(HideBranchesList, SBranchesListWidget)
				.bIsShowed(false)
				.Editor(SmartDialogueEditor)
				.Title(LOCTEXT("HideBranchesLabel", "Hide Branches:"))
			]

			+ SHorizontalBox::Slot()
			  .FillWidth(1.0f)
			  .Padding(4.f, 0.f)
			  .VAlign(VAlign_Top)
			[
				SAssignNew(ShowBranchesList, SBranchesListWidget)
				.bIsShowed(true)
				.Editor(SmartDialogueEditor)
				.Title(LOCTEXT("ShowBranchesLabel", "Show Branches:"))
			]

			+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.Padding(4.f, 0.f)
				.VAlign(VAlign_Top)
			[
				SAssignNew(ModifyVariablesList, SOperationsListWidget)
				.Editor(SmartDialogueEditor)
				.bIsExecution(true)
				.Title(LOCTEXT("ModifyVariablesLabel", "Modify Variables:"))
			]

			+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.Padding(4.f, 0.f)
				.VAlign(VAlign_Top)
			[
				SAssignNew(CheckEntryConditionsList, SOperationsListWidget)
				.Editor(SmartDialogueEditor)
				.bIsExecution(false)
				.Title(LOCTEXT("CheckEntryConditionsLabel", "Check Entry Conditions:"))
			]
		];
	}
	
	return PropertiesContentWidget.ToSharedRef(); 
}

FSmartDialogueBranch* SBranchPropertiesWidget::GetBranchDataPtr()
{
	if (auto SelectedBranchPtr = SmartDialogueEditor.Get()->GetSelectedBranch())
	{
		return SelectedBranchPtr;
	}
	return nullptr;
}

void SBranchPropertiesWidget::OnBranchSelected(FSmartDialogueBranch& SmartDialogueBranch)
{
	if (GetBranchDataPtr() != nullptr)
	{
		ContentWidget = GetContentWidget();
		UpdateWidgets();
	}
	else
	{
		ContentWidget = SNullWidget::NullWidget;
	}
	
	ChildSlot
	[
		ContentWidget.ToSharedRef()
	];}

void SBranchPropertiesWidget::UpdateWidgets()
{
	// Обновление состояния CheckBox-ов
	HiddenCheckBox->SetIsChecked(GetBranchDataPtr()->Hidden ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	HideSelfCheckBox->SetIsChecked(GetBranchDataPtr()->HideSelf ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	ClosedCheckBox->SetIsChecked(GetBranchDataPtr()->Closed ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	ShowChoiceCheckBox->SetIsChecked(GetBranchDataPtr()->Choice ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	CheckAsORCheckBox->SetIsChecked(GetBranchDataPtr()->OrCond ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);

	EventNameTextBox->SetText(FText::FromString(GetBranchDataPtr()->Event.Name));
	
	// AllBranchesList = SmartDialogueEditor.Get()->GetAllBranchesList();
	// for (auto Element : AllBranchesList)
	// {
	// 	if (Element.Get()->Equals(GetBranchDataPtr()->ChangeStarted))
	// 	{
	// 		StartBranchComboBox->SetSelectedItem(Element);
	// 		break;
	// 	}		
	// }

	// Получение данных
	TArray<FString> ShowStrings = GetBranchDataPtr()->Show;
	TArray<FString> HideStrings = GetBranchDataPtr()->Hide;
	TArray<FIf> IfElements = GetBranchDataPtr()->If;
	TArray<FSmartDialogueVars> VarElements = GetBranchDataPtr()->Vars;

	// Преобразование данных для ShowBranchesList и HideBranchesList
	TArray<FListItemData> ShowListItems, HideListItems;
	for (const FString& ShowString : ShowStrings)
	{
		ShowListItems.Add(FListItemData{ ShowString });
	}

	for (const FString& HideString : HideStrings)
	{
		HideListItems.Add(FListItemData{ HideString });
	}

	// Преобразование данных для CheckEntryConditionsList и ModifyVariablesList
	TArray<FListItemData> CheckEntryItems, ModifyVarItems;
	for (const FIf& IfElement : IfElements)
	{
		FListItemData ItemData;
		ItemData.Name = IfElement.Key;
		ItemData.OperationString = UEnum::GetValueAsString<ESmartDialogueEqualOperation>(IfElement.EqualOperation);
		ItemData.Value = IfElement.Value;
		CheckEntryItems.Add(ItemData);
	}

	for (const FSmartDialogueVars& VarElement : VarElements)
	{
		FListItemData ItemData;
		ItemData.Name = VarElement.Key;
		ItemData.OperationString = UEnum::GetValueAsString<ESmartDialogueOperation>(VarElement.Operation);
		ItemData.Value = VarElement.Value;
		ModifyVarItems.Add(ItemData);
	}

	StartBranchComboBox->SetItemValue(GetBranchDataPtr()->ChangeStarted);

	// Обновление виджетов с новыми данными
	ShowBranchesList->UpdateData(ShowListItems);
	HideBranchesList->UpdateData(HideListItems);
	CheckEntryConditionsList->UpdateData(static_cast<TArray<FListItemData>>(CheckEntryItems));
	ModifyVariablesList->UpdateData(static_cast<TArray<FListItemData>>(ModifyVarItems));

}

#undef LOCTEXT_NAMESPACE
