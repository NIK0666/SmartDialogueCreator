// SBranchPropertiesWidget.cpp


#include "SBranchPropertiesWidget.h"

#include "EditorStyleSet.h"
#include "Lists/SBranchesListWidget.h"
#include "Lists/SVerticalListWidget.h"

#define LOCTEXT_NAMESPACE "SmartDialogueEditor"

void SBranchPropertiesWidget::Construct(const FArguments& InArgs)
{
	SmartDialogueEditor = InArgs._SmartDialogueEditor;
	DialogueBranchData = InArgs._DialogueBranchData;

	ChildSlot
	[
		SNew(SVerticalBox)
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
				.IsChecked(DialogueBranchData.Hidden.IsEmpty() ? ECheckBoxState::Unchecked : ECheckBoxState::Checked)
				.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState) { DialogueBranchData.Hidden = NewState == ECheckBoxState::Checked ? TEXT("true") : TEXT(""); })
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
				.IsChecked(DialogueBranchData.HideSelf.IsEmpty() ? ECheckBoxState::Unchecked : ECheckBoxState::Checked)
				.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState) { DialogueBranchData.HideSelf = NewState == ECheckBoxState::Checked ? TEXT("true") : TEXT(""); })
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
				.IsChecked(DialogueBranchData.Closed ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState) { DialogueBranchData.Closed = NewState == ECheckBoxState::Checked; })
				.Content()
				[
					SNew(STextBlock)
					.Margin(FMargin(4.f, 0.f))
					.Text(LOCTEXT("EndOfDialogueLabel", "End of Dialogue"))
				]
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			[
				SAssignNew(EventBox, SBox)
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
						SNew(SEditableTextBox)
						.Text(FText::FromString(DialogueBranchData.Event.Name))
						.OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType) { DialogueBranchData.Event.Name = NewText.ToString(); })
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
							// SmartDialogueEditor->ShowEventSettings(DialogueBranchData.Event);
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
		  .Padding(FMargin(0.0f, 4.0f))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			  .FillWidth(1.0f)
			  .Padding(4.f, 0.f)
			  .VAlign(VAlign_Center)
			[
				SNew(SVerticalListWidget)
				.Title(LOCTEXT("HideBranchesLabel", "Hide Branches:"))
			]

			+ SHorizontalBox::Slot()
			  .FillWidth(1.0f)
			  .Padding(4.f, 0.f)
			  .VAlign(VAlign_Center)
			[
				SNew(SVerticalListWidget)
				.Title(LOCTEXT("ShowBranchesLabel", "Show Branches:"))
			]

			+ SHorizontalBox::Slot()
			  .FillWidth(1.0f)
			  .Padding(4.f, 0.f)
			  .VAlign(VAlign_Center)
			[
				SNew(SVerticalListWidget)
				.Title(LOCTEXT("ModifyVariablesLabel", "Modify Variables:"))
			]

			+ SHorizontalBox::Slot()
			  .FillWidth(1.0f)
			  .Padding(4.f, 0.f)
			  .VAlign(VAlign_Center)
			[
				SNew(SVerticalListWidget)
				.Title(LOCTEXT("CheckEntryConditionsLabel", "Check Entry Conditions:"))
			]
		]
	];
}

#undef LOCTEXT_NAMESPACE