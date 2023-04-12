// SBranchPropertiesWidget.cpp


#include "SBranchPropertiesWidget.h"

#include "FSmartDialogueEditor.h"
#include "SmartDialogue.h"
#include "Components/SBranchComboBox.h"
#include "Lists/SHideBranchesComboBoxList.h"
#include "Lists/SShowBranchesComboBoxList.h"
#include "Lists/SConditionsComboBoxList.h"
#include "Lists/SOperationsComboBoxList.h"
#include "Widgets/Layout/SWrapBox.h"

#define LOCTEXT_NAMESPACE "SmartDialogueEditor"

void SBranchPropertiesWidget::Construct(const FArguments& InArgs) 
{
	SmartDialogueEditor = InArgs._SmartDialogueEditor;

	SmartDialogueEditor->OnBranchSelected.AddSP(this, &SBranchPropertiesWidget::OnBranchSelected);
	SmartDialogueEditor->OnResetSelectedBranch.AddSP(this, &SBranchPropertiesWidget::OnResetSelectedBranch);
	SmartDialogueEditor->OnBranchItemRemoved.AddSP(this, &SBranchPropertiesWidget::OnBranchItemRemoved);

	SmartDialogueEditor->GetDialogue()->OnShowBranchRemoved.AddSP(this, &SBranchPropertiesWidget::OnShowBranchRemoved);
	SmartDialogueEditor->GetDialogue()->OnShowBranchAdded.AddSP(this, &SBranchPropertiesWidget::OnShowBranchAdded);
	SmartDialogueEditor->GetDialogue()->OnHideBranchRemoved.AddSP(this, &SBranchPropertiesWidget::OnHideBranchRemoved);
	SmartDialogueEditor->GetDialogue()->OnHideBranchAdded.AddSP(this, &SBranchPropertiesWidget::OnHideBranchAdded);
	SmartDialogueEditor->GetDialogue()->OnBranchRenamed.AddSP(this, &SBranchPropertiesWidget::OnBranchRenamed);
	
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
			.FillWidth(1.f)
			.Padding(4.f, 0.f)
			.VAlign(VAlign_Center)
			[
				SNew(SWrapBox)
				.UseAllottedSize(true)
				+ SWrapBox::Slot()
				.Padding(4.f, 0.f)
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
				+ SWrapBox::Slot()
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

				+ SWrapBox::Slot()
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
				+ SWrapBox::Slot()
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
				
				// "QueuePhrases"
				+ SWrapBox::Slot()
				.Padding(4.f, 0.f)
				.VAlign(VAlign_Center)
				[
					SAssignNew(QueueCheckBox, SCheckBox)
					.HAlign(HAlign_Left)
					.IsChecked(GetBranchDataPtr()->Queue ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
					.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
					{
						GetBranchDataPtr()->Queue = NewState == ECheckBoxState::Checked;
						if (SmartDialogueEditor->GetSelectedBranch())
						{
							SmartDialogueEditor->GetSelectedBranch()->Queue = NewState == ECheckBoxState::Checked;
						}
					})
					.Content()
					[
						SNew(STextBlock)
						.Margin(FMargin(4.f, 0.f))
						.Text(LOCTEXT("QueuePhrasesLabel", "Queue"))
					]
				]

				// "Check as OR"
				+ SWrapBox::Slot()
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
				+ SWrapBox::Slot()
				[
					SAssignNew(StartBranchComboBox, SBranchComboBox)
					.SmartDialogueEditor(SmartDialogueEditor)
					.OnItemSelected_Lambda([this](TSharedPtr<FString> NewSelection)
					{
						GetBranchDataPtr()->ChangeStarted = *NewSelection;
					})
					.DefaultText(SmartDialogueEditor->GetSelectedBranch()->ChangeStarted)
				]					
			]	
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Right)
			[
				SNew(SVerticalBox)				
				+SVerticalBox::Slot()
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
						.OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType)
						{
							auto L_Event = GetBranchDataPtr()->Event;
							L_Event.Name = NewText.ToString();
							SmartDialogueEditor->GetDialogue()->UpdateEventInfo(SmartDialogueEditor->GetSelectedBranchName(), L_Event);							
						})
						.HintText(LOCTEXT("EventNameLabel", "Event name"))
					]

					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						SAssignNew(EventParamTextBox, SEditableTextBox)
						.Text(FText::FromString(GetBranchDataPtr()->Event.Param))
						.OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType)
						{
							auto L_Event = GetBranchDataPtr()->Event;
							L_Event.Param = NewText.ToString();
							SmartDialogueEditor->GetDialogue()->UpdateEventInfo(SmartDialogueEditor->GetSelectedBranchName(), L_Event);	
						})
						.HintText(LOCTEXT("EventParamLabel", "Event param"))
						.Visibility(this, &SBranchPropertiesWidget::GetEventParamVisibility)
					]

					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						SAssignNew(AfterBranchCheckBox, SCheckBox)
						.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
						{
							auto L_Event = GetBranchDataPtr()->Event;
							L_Event.Post = (NewState == ECheckBoxState::Checked);
							SmartDialogueEditor->GetDialogue()->UpdateEventInfo(SmartDialogueEditor->GetSelectedBranchName(), L_Event);	
						})
						.ToolTipText(LOCTEXT("AfterAllPhrasesTooltip", "After all phrases"))
						.Visibility(this, &SBranchPropertiesWidget::GetEventParamVisibility)
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
				SAssignNew(HideBranchesList, SHideBranchesComboBoxList)
				.InitialStrings(SmartDialogueEditor->GetSelectedBranch()->Hide)
				.Editor(SmartDialogueEditor)
				.Title(LOCTEXT("HideBranchesLabel", "Hide Branches:"))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(2.f)
				.HeightOverride(100.f)
				[
					SNew(SBorder)
					.BorderImage(FAppStyle::GetBrush("WhiteTexture"))
					.BorderBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.2f))
				]
			]
			+ SHorizontalBox::Slot()
			  .FillWidth(1.0f)
			  .Padding(4.f, 0.f)
			  .VAlign(VAlign_Top)
			[
				SAssignNew(ShowBranchesList, SShowBranchesComboBoxList)
				.InitialStrings(SmartDialogueEditor->GetSelectedBranch()->Show)
				.Editor(SmartDialogueEditor)
				.Title(LOCTEXT("ShowBranchesLabel", "Show Branches:"))
			]
		]
		+ SVerticalBox::Slot()
		  .AutoHeight()
		  .HAlign(HAlign_Fill)
		  .VAlign(VAlign_Top)
		  .Padding(FMargin(0.0f, 0.0f))
		[
			SNew(SBox)
			.HeightOverride(2.f)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("WhiteTexture"))
				.BorderBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.2f))
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
				SAssignNew(ModifyVariablesList, SOperationsComboBoxList)
				.InitialVars(SmartDialogueEditor->GetSelectedBranch()->Vars)
				.Editor(SmartDialogueEditor)
				.Title(LOCTEXT("ModifyVariablesLabel", "Modify Variables:"))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(2.f)
				.HeightOverride(100.f)
				[
					SNew(SBorder)
					.BorderImage(FAppStyle::GetBrush("WhiteTexture"))
					.BorderBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.2f))
				]
			]
			+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.Padding(4.f, 0.f)
				.VAlign(VAlign_Top)
			[
				SAssignNew(CheckEntryConditionsList, SConditionsComboBoxList)
				.InitialConditions(SmartDialogueEditor->GetSelectedBranch()->If)
				.Editor(SmartDialogueEditor)
				.Title(LOCTEXT("CheckEntryConditionsLabel", "Check Entry Conditions:"))
			]
		];
	}
	
	return PropertiesContentWidget.ToSharedRef(); 
}

FSmartDialogueBranch* SBranchPropertiesWidget::GetBranchDataPtr()
{ 
	if (auto SelectedBranchPtr = SmartDialogueEditor->GetSelectedBranch())
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
	];
}

void SBranchPropertiesWidget::OnResetSelectedBranch()
{
	ContentWidget = SNullWidget::NullWidget;
	
	ChildSlot
	[
		ContentWidget.ToSharedRef()
	];
}

void SBranchPropertiesWidget::OnBranchItemRemoved(FName& Name)
{
	
}

void SBranchPropertiesWidget::OnShowBranchRemoved(FName BranchName, int32 Index, FString Value)
{
	if (SmartDialogueEditor->GetSelectedBranchName() == BranchName)
	{
		ShowBranchesList->UpdateInitialStrings(GetBranchDataPtr()->Show);
	}
}

void SBranchPropertiesWidget::OnShowBranchAdded(FName BranchName, FString String)
{
	if (SmartDialogueEditor->GetSelectedBranchName() == BranchName)
	{
		ShowBranchesList->UpdateInitialStrings(GetBranchDataPtr()->Show);
	}
}

void SBranchPropertiesWidget::OnHideBranchRemoved(FName BranchName, int32 Index, FString Value)
{
	if (SmartDialogueEditor->GetSelectedBranchName() == BranchName)
	{
		HideBranchesList->UpdateInitialStrings(GetBranchDataPtr()->Hide);
	}
}

void SBranchPropertiesWidget::OnHideBranchAdded(FName BranchName, FString String)
{
	if (SmartDialogueEditor->GetSelectedBranchName() == BranchName)
	{
		HideBranchesList->UpdateInitialStrings(GetBranchDataPtr()->Hide);
	}
}

void SBranchPropertiesWidget::OnBranchRenamed(FName OldName, FName NewName)
{
	ShowBranchesList->UpdateInitialStrings(GetBranchDataPtr()->Show);
	HideBranchesList->UpdateInitialStrings(GetBranchDataPtr()->Hide);
}


void SBranchPropertiesWidget::UpdateWidgets()
{
	// Обновление состояния CheckBox-ов
	HiddenCheckBox->SetIsChecked(GetBranchDataPtr()->Hidden ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	HideSelfCheckBox->SetIsChecked(GetBranchDataPtr()->HideSelf ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	ClosedCheckBox->SetIsChecked(GetBranchDataPtr()->Closed ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	ShowChoiceCheckBox->SetIsChecked(GetBranchDataPtr()->Choice ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	QueueCheckBox->SetIsChecked(GetBranchDataPtr()->Queue ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	CheckAsORCheckBox->SetIsChecked(GetBranchDataPtr()->OrCond ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);

	EventNameTextBox->SetText(FText::FromString(GetBranchDataPtr()->Event.Name));
	EventParamTextBox->SetText(FText::FromString(GetBranchDataPtr()->Event.Param));
	AfterBranchCheckBox->SetIsChecked(GetBranchDataPtr()->Event.Post);

	StartBranchComboBox->SetItemValue(GetBranchDataPtr()->ChangeStarted);

	// Обновление виджетов с новыми данными
	ShowBranchesList->UpdateInitialStrings(GetBranchDataPtr()->Show);
	HideBranchesList->UpdateInitialStrings(GetBranchDataPtr()->Hide);
	CheckEntryConditionsList->UpdateInitialConditions(GetBranchDataPtr()->If);
	ModifyVariablesList->UpdateInitialVars(GetBranchDataPtr()->Vars);

}

EVisibility SBranchPropertiesWidget::GetEventParamVisibility() const
{	
	return SmartDialogueEditor->GetSelectedBranch()->Event.Name.IsEmpty() ? EVisibility::Collapsed : EVisibility::Visible;
}

#undef LOCTEXT_NAMESPACE
