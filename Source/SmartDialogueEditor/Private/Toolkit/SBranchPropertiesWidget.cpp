// 


#include "SBranchPropertiesWidget.h"

#include "EditorStyleSet.h"

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
			[
				SAssignNew(HiddenCheckBox, SCheckBox)
				.IsChecked(DialogueBranchData.Hidden.IsEmpty() ? ECheckBoxState::Unchecked : ECheckBoxState::Checked)
				.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState) { DialogueBranchData.Hidden = NewState == ECheckBoxState::Checked ? TEXT("true") : TEXT(""); })
				.Content()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("HiddenLabel", "Hidden"))
				]
			]

			+ SHorizontalBox::Slot()
			[
				SAssignNew(HideSelfCheckBox, SCheckBox)
				.IsChecked(DialogueBranchData.HideSelf.IsEmpty() ? ECheckBoxState::Unchecked : ECheckBoxState::Checked)
				.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState) { DialogueBranchData.HideSelf = NewState == ECheckBoxState::Checked ? TEXT("true") : TEXT(""); })
				.Content()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("HideSelfLabel", "Hide Self"))
				]
			]

			+ SHorizontalBox::Slot()
			[
				SAssignNew(ClosedCheckBox, SCheckBox)
				.IsChecked(DialogueBranchData.Closed ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState) { DialogueBranchData.Closed = NewState == ECheckBoxState::Checked; })
				.Content()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("EndOfDialogueLabel", "End of Dialogue"))
				]
			]
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			[
				SAssignNew(EventBox, SBox)
				.HAlign(HAlign_Fill)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("EventLabel", "Event:"))
						.Margin(FMargin(0.0f, 0.0f, 5.0f, 0.0f))
					]

					+ SHorizontalBox::Slot()
					[
						SNew(SEditableTextBox)
						.Text(FText::FromString(DialogueBranchData.Event.Name))
						.OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType) { DialogueBranchData.Event.Name = NewText.ToString(); })
						.HintText(LOCTEXT("EventNameLabel", "Event name"))
					]

					+ SHorizontalBox::Slot()
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
							SNew(SImage)
							.Image(FEditorStyle::GetBrush("Icons.Settings"))
						]
					]
				]
			]
		]
	];
}

#undef LOCTEXT_NAMESPACE