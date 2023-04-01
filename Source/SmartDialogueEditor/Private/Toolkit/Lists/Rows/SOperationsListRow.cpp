// SOperationsListRow.cpp


#include "SOperationsListRow.h"

#include "SmartDialogue.h"
#include "Toolkit/FSmartDialogueEditor.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SEditableText.h"

void SOperationsListRow::Construct(const FArguments& InArgs)
{
	bIsExecution = InArgs._bIsExecution;
	SBaseListRow::Construct(SBaseListRow::FArguments()
		.Item(InArgs._Item)
		.OnRemoveItemRequested(InArgs._OnRemoveItemRequested)
		.Editor(InArgs._Editor));	
}

TSharedRef<SWidget> SOperationsListRow::GetContent()
{
	// Initialize the VariableOptions array
	VariableOptions = Editor->GetAllVariablesList();

	// Initialize the OperationOptions array
	OperationOptions = TArray<TSharedPtr<FString>>();
	TArray<TSharedPtr<FString>> Operations = Editor->GetOperations(bIsExecution);
	for (TSharedPtr<FString> Op : Operations)
	{
		OperationOptions.Add(Op);
	}

	// FListItemData* pItem = &Item;
	// if (FListItemData* pOperationItem = static_cast<FListItemData*>(pItem))
	// {
	// 	OperationItem = *pOperationItem;
	// }

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			[
				SAssignNew(VariableComboBox, SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&VariableOptions)
				.OnGenerateWidget_Lambda([](TSharedPtr<FString> InItem)
				{
					return SNew(STextBlock).Text(FText::FromString(*InItem));
				})
				.OnSelectionChanged(this, &SOperationsListRow::OnVariableSelected)
				.Content()
				[
					SAssignNew(VariableButtonText, STextBlock).Text(FText::FromString(Item.Name))
				]
				.ToolTipText_Lambda([this]()
				{
					return SelectedVariable.IsValid() ? FText::FromString(*SelectedVariable) : FText::FromString(TEXT("Variable"));
				})
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(OperationComboBox, SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&OperationOptions)
				.OnGenerateWidget_Lambda([](TSharedPtr<FString> InItem)
				{
					return SNew(STextBlock).Text(FText::FromString(*InItem));
				})
				.OnSelectionChanged(this, &SOperationsListRow::OnOperationSelected)
				.Content()
				[
					SAssignNew(OperationButtonText, STextBlock).Text(OperationOptions.Num() > 0 ? FText::FromString(*OperationOptions[0]) : FText::FromString(TEXT("")))
				]
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SAssignNew(InputTextBox, SEditableTextBox)
				.Text(FText::FromString(FString::FromInt(Item.Value)))
				.HintText(FText::FromString(TEXT("value")))
			]
		];
}

void SOperationsListRow::OnVariableSelected(TSharedPtr<FString> SelectedItem, ESelectInfo::Type SelectInfo)
{
	if (SelectedItem.IsValid())
	{
		SelectedVariable = SelectedItem;
		UE_LOG(LogTemp, Log, TEXT("Variable selected: %s"), **SelectedItem);
		VariableButtonText->SetText(FText::FromString(*SelectedItem));
	}
}

void SOperationsListRow::OnOperationSelected(TSharedPtr<FString> SelectedItem, ESelectInfo::Type SelectInfo)
{
	if (SelectedItem.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Operation selected: %s"), **SelectedItem);
		OperationButtonText->SetText(FText::FromString(*SelectedItem));

		if (bIsExecution)
		{
			if (const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESmartDialogueOperation"), true))
			{
				SelectedEnumIndex = EnumPtr->GetIndexByName(FName(**SelectedItem));
			}
		}
		else
		{
			if (const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESmartDialogueEqualOperation"), true))
			{
				SelectedEnumIndex = EnumPtr->GetIndexByName(FName(**SelectedItem));
			}
		}
	}
}

FReply SOperationsListRow::RemoveItem()
{
	return SBaseListRow::RemoveItem();
}
