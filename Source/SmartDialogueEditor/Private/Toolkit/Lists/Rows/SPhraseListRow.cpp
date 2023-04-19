// SPhraseListRow.cpp


#include "SPhraseListRow.h"

#include "Toolkit/FSmartDialogueEditor.h"
#include "Toolkit/Components/SCharacterComboBox.h"
#include "Toolkit/Components/SVarComboBox.h"
#include "Toolkit/DragDrop/PhraseDragDropOperation.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SGridPanel.h"


void SPhraseListRow::Construct(const FArguments& InArgs)
{
	SmartDialogueEditor = InArgs._SmartDialogueEditor;
	PhraseIndex = InArgs._PhraseIndex;
	
	CharacterOptions = SmartDialogueEditor->GetAllCharactersList();
	VarOptions = SmartDialogueEditor->GetAllVariablesList();

	ComparisonOptions = SmartDialogueEditor->GetOperations(false);
	auto InitOp = ComparisonOptions[0];
	const auto StringOp = ESmartDialogueEqualOperationHelper::EnumOperationToString(GetPhrasePtr()->If.EqualOperation);
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
		SNew(SBorder)
		.Padding(2.f)
		.BorderImage(FCoreStyle::Get().GetBrush("WhiteTexture"))
		.BorderBackgroundColor(this, &SPhraseListRow::GetBackgroundColor)
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
					.DefaultText(GetPhrasePtr()->NPC)
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
					.DefaultText(GetPhrasePtr()->If.Key)
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
					SAssignNew(GrabButton, SButton)
					.ButtonStyle(FAppStyle::Get(), "FlatButton")
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.IsFocusable(false)
					.Visibility(EVisibility::HitTestInvisible)
					.Content()
					[
						SNew(SImage)
						.Image(FAppStyle::GetBrush("SoftwareCursor_Grab"))
					]
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
		]
	];

	UpdateCustomParamsGrid();
}

FSmartDialoguePhrase* SPhraseListRow::GetPhrasePtr() const
{
	if (SmartDialogueEditor->GetSelectedBranch() && SmartDialogueEditor->GetSelectedBranch()->Phrases.Num() > 0)
	{
		return &SmartDialogueEditor->GetSelectedBranch()->Phrases[PhraseIndex];
	}
	return nullptr;
}

void SPhraseListRow::UnderDragState(bool bIsNewState)
{
	bUnderDrag = bIsNewState;
}

void SPhraseListRow::DraggedState(bool bIsNewState)
{
	bDragged = bIsNewState;
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
    GetPhrasePtr()->NPC = *NewSelection;
}

void SPhraseListRow::OnVarSelected(TSharedPtr<FString> NewSelection)
{
	
    GetPhrasePtr()->If.Key = *NewSelection;
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
	if (GetPhrasePtr()->CustomParams.Contains(ParamKey))
	{
		if (Text.IsEmpty())
		{
			GetPhrasePtr()->CustomParams.Remove(ParamKey);
		}
		else
		{
			GetPhrasePtr()->CustomParams[ParamKey] = Text.ToString();
		}
	}
	else
	{
		if (!Text.IsEmpty())
		{
			GetPhrasePtr()->CustomParams.Add(ParamKey, Text.ToString());
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
		FString* PhraseParamValue = GetPhrasePtr()->CustomParams.Find(CustomParam.Key);
		
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

FReply SPhraseListRow::OnDeleteButtonClicked()
{
	SmartDialogueEditor->RemovePhrase(PhraseIndex);
	return FReply::Handled();
}

void SPhraseListRow::OnMultiLineTextChanged(const FText& InText)
{
	GetPhrasePtr()->Text = InText;
}

FText SPhraseListRow::GetCurrentText() const
{
	if (!GetPhrasePtr())
	{
		return FText::GetEmpty();
	}
	return GetPhrasePtr()->Text;
}

FText SPhraseListRow::GetCurrentCharacterText() const
{
	if (!GetPhrasePtr())
	{
		return FText::GetEmpty();
	}
	return FText::FromString(GetPhrasePtr()->NPC);
}

FText SPhraseListRow::GetCurrentVarText() const
{
	if (!GetPhrasePtr())
	{
		return FText::GetEmpty();
	}
	return FText::FromString(GetPhrasePtr()->If.Key);
}

FText SPhraseListRow::GetCurrentComparisonText() const
{
	return FText::FromString(*ComparisonComboBox->GetSelectedItem());
}

FText SPhraseListRow::GetAnimationText() const
{
	if (!GetPhrasePtr())
	{
		return FText::GetEmpty();
	}
	return FText::FromString(*GetPhrasePtr()->Anim);
}

void SPhraseListRow::OnAnimationTextChanged(const FText& Text)
{
	GetPhrasePtr()->Anim = Text.ToString();
}

FText SPhraseListRow::GetCompareValueText() const
{
	return FText::FromString(FString::FromInt(GetPhrasePtr()->If.Value));
}

void SPhraseListRow::OnCompareValueTextChanged(const FText& Text)
{
	FIf* ComparePtr = &GetPhrasePtr()->If;
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
	if (!GetPhrasePtr())
	{
		return EVisibility::Collapsed;
	}
	return GetPhrasePtr()->If.Key.IsEmpty() ? EVisibility::Collapsed : EVisibility::Visible;
}

FSlateColor SPhraseListRow::GetBackgroundColor() const
{
	if (!GetPhrasePtr())
	{
		return FLinearColor::Transparent;
	}
	if (bUnderDrag)
	{
		return FLinearColor(0.34f, 0.34f, 0.34f, 1.f);
	}
	if (bDragged)
	{
		return FLinearColor(0.88f, 0.88f, 0.88f, 1.f);
	}

	return FLinearColor::Transparent;
}

FReply SPhraseListRow::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		FGeometry GrabButtonGeometry = FindChildGeometry(MyGeometry, GrabButton.ToSharedRef());

		if (GrabButtonGeometry.IsUnderLocation(MouseEvent.GetScreenSpacePosition()))
		{
			const TSharedRef<FPhraseDragDropOperation> DragDropOp = FPhraseDragDropOperation::New(SharedThis(this), SmartDialogueEditor->GetPhrasesListPanel());
			DraggedState(true);
			return FReply::Handled().BeginDragDrop(DragDropOp);
		}
	}

	return SCompoundWidget::OnMouseButtonDown(MyGeometry, MouseEvent);
}