// SPhraseListRow.cpp


#include "SPhraseListRow.h"

#include "Toolkit/FSmartDialogueEditor.h"
#include "Toolkit/Components/SCharacterComboBox.h"
#include "Toolkit/Components/SVarComboBox.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SGridPanel.h"


void SPhraseListRow::Construct(const FArguments& InArgs)
{
	SmartDialogueEditor = InArgs._SmartDialogueEditor;
	PhraseIndex = InArgs._PhraseIndex;

	SmartDialoguePhrasePtr = &SmartDialogueEditor->GetSelectedBranch()->Phrases[PhraseIndex];
	
	CharacterOptions = SmartDialogueEditor->GetAllCharactersList();
	VarOptions = SmartDialogueEditor->GetAllVariablesList();

	ComparisonOptions = SmartDialogueEditor->GetOperations(false);
	auto InitOp = ComparisonOptions[0];
	const auto StringOp = ESmartDialogueEqualOperationHelper::EnumOperationToString(SmartDialoguePhrasePtr->If.EqualOperation);
	for (auto Op : ComparisonOptions)
	{
		if (Op->Equals(StringOp))
		{
			InitOp = Op;
			break;
		}
	}
	
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(CharacterComboBox, SCharacterComboBox)
				.SmartDialogueEditor(SmartDialogueEditor)
				.OnItemSelected(this, &SPhraseListRow::OnCharacterSelected)
				.DefaultText(SmartDialoguePhrasePtr->NPC)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SEditableTextBox)
				.HintText(NSLOCTEXT("SPhraseListRow", "Animation", "Animation"))
				.Text(this, &SPhraseListRow::GetAnimationText)
				.OnTextChanged(this, &SPhraseListRow::OnAnimationTextChanged)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(VarComboBox, SVarComboBox)
				.SmartDialogueEditor(SmartDialogueEditor)
				.OnItemSelected(this, &SPhraseListRow::OnVarSelected)
				.DefaultText(SmartDialoguePhrasePtr->If.Key)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(ComparisonComboBox, SComboBox<TSharedPtr<FString>>)
				.ContentPadding(FMargin(2.0f))
				.OptionsSource(&ComparisonOptions)
				.OnGenerateWidget(this, &SPhraseListRow::GenerateComparisonOption)
				.OnSelectionChanged(this, &SPhraseListRow::OnComparisonSelected)
				.Visibility(this, &SPhraseListRow::GetComparisonVisibility)
				.InitiallySelectedItem(InitOp)
				[
					SAssignNew(ComparisonTextBlock, STextBlock)
					.Text(this, &SPhraseListRow::GetCurrentComparisonText)
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SEditableTextBox)
				.HintText(NSLOCTEXT("SPhraseListRow", "Value", "Value"))
				.Text(this, &SPhraseListRow::GetCompareValueText)
				.Visibility(this, &SPhraseListRow::GetComparisonVisibility)
				.OnTextChanged(this, &SPhraseListRow::OnCompareValueTextChanged)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged(this, &SPhraseListRow::OnOrCheckStateChanged)
				.Visibility(this, &SPhraseListRow::GetComparisonVisibility)
				.Content()
				[
					SNew(STextBlock)
					.Text(NSLOCTEXT("SPhraseListRow", "Or", "Or?"))
				]
			]
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(SBox)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(CustomParamsGrid, SGridPanel)
				
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.ButtonStyle(FAppStyle::Get(), "FlatButton")
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Content()
				[
					SNew(SImage)
					.Image(FAppStyle::GetBrush("SoftwareCursor_Grab"))
				]
				.OnClicked(this, &SPhraseListRow::OnHandButtonClicked)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.ButtonStyle(FAppStyle::Get(), "FlatButton")
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Content()
				[
					SNew(SImage)
					.Image(FAppStyle::GetBrush("Cross"))
				]
				.OnClicked(this, &SPhraseListRow::OnDeleteButtonClicked)
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.WidthOverride(300.0f)
			[
			SAssignNew(MultiLineEditableTextBox, SMultiLineEditableTextBox)
			.AutoWrapText(true)
			.Text(this, &SPhraseListRow::GetCurrentText)
			.OnTextChanged(this, &SPhraseListRow::OnMultiLineTextChanged)
			.OnKeyDownHandler(this, &SPhraseListRow::OnMultiLineKeyDown) // Добавляем обработчик клавиш
			]
		]
	];

	UpdateCustomParamsGrid();
}

FSmartDialoguePhrase* SPhraseListRow::GetPhrasePtr() const
{
	
	return SmartDialoguePhrasePtr;
}


TSharedRef<SWidget> SPhraseListRow::GenerateCharacterOption(TSharedPtr<FString> Option)
{
	return SNew(STextBlock).Text(FText::FromString(*Option));
}

TSharedRef<SWidget> SPhraseListRow::GenerateVarOption(TSharedPtr<FString> Option)
{
	return SNew(STextBlock).Text(FText::FromString(*Option));
}

TSharedRef<SWidget> SPhraseListRow::GenerateComparisonOption(TSharedPtr<FString> Option)
{
	return SNew(STextBlock).Text(FText::FromString(*Option));
}

void SPhraseListRow::OnCharacterSelected(TSharedPtr<FString> NewSelection)
{
    SmartDialoguePhrasePtr->NPC = *NewSelection;
}

void SPhraseListRow::OnVarSelected(TSharedPtr<FString> NewSelection)
{
	
    SmartDialoguePhrasePtr->If.Key = *NewSelection;
}

void SPhraseListRow::OnComparisonSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
    ComparisonTextBlock->SetText(FText::FromString(*NewSelection));
}

void SPhraseListRow::OnOrCheckStateChanged(ECheckBoxState NewState)
{
	// Обработка изменения состояния чекбокса "Or?"
}

void SPhraseListRow::OnParameterChanged(const FText& Text, FString ParamKey)
{
	if (SmartDialoguePhrasePtr->CustomParams.Contains(ParamKey))
	{
		if (Text.IsEmpty())
		{
			SmartDialoguePhrasePtr->CustomParams.Remove(ParamKey);
		}
		else
		{
			SmartDialoguePhrasePtr->CustomParams[ParamKey] = Text.ToString();
		}
	}
	else
	{
		if (!Text.IsEmpty())
		{
			SmartDialoguePhrasePtr->CustomParams.Add(ParamKey, Text.ToString());
		}
	}
}

void SPhraseListRow::UpdateCustomParamsGrid()
{
	TArray<FCustomParameterData> AllCustomParams = SmartDialogueEditor->GetDialogueConfig()->GetCustomParameters();
	
	CustomParamsGrid->ClearChildren();

	int32 CurrentColumn = 0;
	for (auto CustomParam : AllCustomParams)
	{
		FString* PhraseParamValue = SmartDialoguePhrasePtr->CustomParams.Find(CustomParam.Key);
		
		TSharedRef<SEditableTextBox> EditableTextBox = SNew(SEditableTextBox)
			.HintText(FText::FromString(CustomParam.Key))
			.ToolTipText(FText::FromString(CustomParam.Key))
			.Text(FText::FromString(PhraseParamValue != nullptr ? *PhraseParamValue : ""))
			.OnTextChanged(this, &SPhraseListRow::OnParameterChanged, CustomParam.Key);

		CustomParamsGrid->AddSlot(CurrentColumn, 0)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			[
				EditableTextBox
			];

		CurrentColumn++;
	}
}

FReply SPhraseListRow::OnHandButtonClicked()
{
	// Обработка нажатия на кнопку с иконкой руки
	return FReply::Handled();
}

FReply SPhraseListRow::OnDeleteButtonClicked()
{
	SmartDialogueEditor->RemovePhrase(PhraseIndex);
	return FReply::Handled();
}

void SPhraseListRow::OnMultiLineTextChanged(const FText& InText)
{
	SmartDialoguePhrasePtr->Text = InText;
}

FText SPhraseListRow::GetCurrentText() const
{
	return SmartDialoguePhrasePtr->Text;
}

FText SPhraseListRow::GetCurrentCharacterText() const
{
	return FText::FromString(SmartDialoguePhrasePtr->NPC);
}

FText SPhraseListRow::GetCurrentVarText() const
{
	return FText::FromString(SmartDialoguePhrasePtr->If.Key);
}

FText SPhraseListRow::GetCurrentComparisonText() const
{
	return FText::FromString(*ComparisonComboBox->GetSelectedItem());
}

FText SPhraseListRow::GetAnimationText() const
{
	return FText::FromString(*SmartDialoguePhrasePtr->Anim);
}

void SPhraseListRow::OnAnimationTextChanged(const FText& Text)
{
	SmartDialoguePhrasePtr->Anim = Text.ToString();
}

FText SPhraseListRow::GetCompareValueText() const
{
	return FText::FromString(FString::FromInt(SmartDialoguePhrasePtr->If.Value));
}

void SPhraseListRow::OnCompareValueTextChanged(const FText& Text)
{
	FIf* ComparePtr = &SmartDialoguePhrasePtr->If;
	ComparePtr->Value = FCString::Atoi(*Text.ToString());
}

FReply SPhraseListRow::OnMultiLineKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Enter)
	{
		if (!InKeyEvent.IsShiftDown())
		{
			// Заканчиваем ввод и снимаем фокус с виджета
			FSlateApplication::Get().ClearKeyboardFocus(EFocusCause::Cleared);
			return FReply::Handled();
		}
		
		// Добавляем перенос строки
		MultiLineEditableTextBox->InsertTextAtCursor(FText::FromString(TEXT("\n")));
		return FReply::Handled();
	}
    
	return FReply::Unhandled();
}

EVisibility SPhraseListRow::GetComparisonVisibility() const
{
	return SmartDialoguePhrasePtr->If.Key.IsEmpty() ? EVisibility::Collapsed : EVisibility::Visible;
}
