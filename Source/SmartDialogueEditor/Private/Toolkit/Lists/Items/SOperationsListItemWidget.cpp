// SOperationsListItemWidget.cpp


#include "SOperationsListItemWidget.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SEditableText.h"

void SOperationsListItemWidget::Construct(const FArguments& InArgs)
{
	bIsExecution = InArgs._bIsExecution;
	SBaseListItemWidget::Construct(SBaseListItemWidget::FArguments());
	
}

TSharedRef<SWidget> SOperationsListItemWidget::GetContent()
{
	// Initialize the VariableOptions array
	VariableOptions = TArray<TSharedPtr<FString>>();
	TArray<FString> ValueNames = GetValueNames();
	for (FString Name : ValueNames)
	{
		VariableOptions.Add(MakeShareable(new FString(Name)));
	}

	// Initialize the OperationOptions array
	OperationOptions = TArray<TSharedPtr<FString>>();
	TArray<TSharedPtr<FString>> Operations = GetOperations();
	for (TSharedPtr<FString> Op : Operations)
	{
		OperationOptions.Add(Op);
	}

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SAssignNew(VariableComboBox, SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&VariableOptions)
				.OnGenerateWidget_Lambda([](TSharedPtr<FString> InItem)
				{
					return SNew(STextBlock).Text(FText::FromString(*InItem));
				})
				.OnSelectionChanged(this, &SOperationsListItemWidget::OnVariableSelected)
				.Content()
				[
					SAssignNew(VariableButtonText, STextBlock).Text(FText::FromString(TEXT("[empty]")))
				]
				.ToolTipText_Lambda([this]()
				{
					return SelectedVariable.IsValid() ? FText::FromString(*SelectedVariable) : FText::FromString(TEXT("[empty]"));
				})
			]
			+ SHorizontalBox::Slot()
			[
				SAssignNew(OperationComboBox, SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&OperationOptions)
				.OnGenerateWidget_Lambda([](TSharedPtr<FString> InItem)
				{
					return SNew(STextBlock).Text(FText::FromString(*InItem));
				})
				.OnSelectionChanged(this, &SOperationsListItemWidget::OnOperationSelected)
				.Content()
				[
					SAssignNew(OperationButtonText, STextBlock).Text(OperationOptions.Num() > 0 ? FText::FromString(*OperationOptions[0]) : FText::FromString(TEXT("")))
				]
			]
			+ SHorizontalBox::Slot()
			[
				SAssignNew(InputTextBox, SEditableTextBox)
				.HintText(FText::FromString(TEXT("value")))
			]
		];
}

TArray<FString> SOperationsListItemWidget::GetValueNames()
{
	return TArray<FString>({ "var1", "variable2", "test_var_3" });
}

TArray<TSharedPtr<FString>> SOperationsListItemWidget::GetOperations()
{
	TArray<TSharedPtr<FString>> Result;

	const UEnum* EnumPtr = nullptr;
	if (bIsExecution)
	{
		EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESmartDialogueOperation"), true);
	}
	else
	{
		EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESmartDialogueEqualOperation"), true);
	}

	if (EnumPtr)
	{
		for (int32 i = 0; i < EnumPtr->NumEnums() - 1; i++)
		{
			FString DisplayName = EnumPtr->GetDisplayNameTextByIndex(i).ToString();
			Result.Add(MakeShareable(new FString(DisplayName)));
		}
	}

	return Result;
}

void SOperationsListItemWidget::OnVariableSelected(TSharedPtr<FString> SelectedItem, ESelectInfo::Type SelectInfo)
{
	if (SelectedItem.IsValid())
	{
		SelectedVariable = SelectedItem;
		UE_LOG(LogTemp, Log, TEXT("Variable selected: %s"), **SelectedItem);
		VariableButtonText->SetText(FText::FromString(*SelectedItem));
	}
}

void SOperationsListItemWidget::OnOperationSelected(TSharedPtr<FString> SelectedItem, ESelectInfo::Type SelectInfo)
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

FReply SOperationsListItemWidget::RemoveItem()
{
	UE_LOG(LogTemp, Log, TEXT("SOperationsListItemWidget::RemoveItem"));
	return SBaseListItemWidget::RemoveItem();
}
