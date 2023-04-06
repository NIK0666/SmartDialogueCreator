// 

#include "SConditionsComboBoxList.h"
#include "SmartDialogue.h"

void SConditionsComboBoxList::Construct(const FArguments& InArgs)
{

    InitialConditions = InArgs._InitialConditions;
    Editor = InArgs._Editor;
    const FText Title = InArgs._Title;

    ChildSlot
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            [
                SNew(STextBlock)
                .Text(Title)
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                SNew(SButton)
                .Text(FText::FromString("Add"))
                .OnClicked(this, &SConditionsComboBoxList::OnAddButtonClicked)
            ]
        ]
        + SVerticalBox::Slot()
        [
            SAssignNew(ListBox, SVerticalBox)
        ]
    ];

    RefreshList();
}

void SConditionsComboBoxList::UpdateInitialConditions(const TArray<FIf>& NewInitialConditions)
{
    InitialConditions = NewInitialConditions;
    RefreshList();
}

FString SConditionsComboBoxList::EnumOperationToString(ESmartDialogueEqualOperation Operation)
{
    switch (Operation)
    {
    case ESmartDialogueEqualOperation::EEO_Equals:
        return "==";
    case ESmartDialogueEqualOperation::EEO_Greater:
        return ">";
    case ESmartDialogueEqualOperation::EEO_Less:
        return "<";
    case ESmartDialogueEqualOperation::EEO_GreaterOrEquals:
        return ">=";
    case ESmartDialogueEqualOperation::EEO_LessOrEquals:
        return "<=";
    }
    
    return "";
}

ESmartDialogueEqualOperation SConditionsComboBoxList::EnumOperationFromString(FString& OperationString)
{
    if (OperationString == "==")
    {
        return ESmartDialogueEqualOperation::EEO_Equals;
    }
    if (OperationString == ">")
    {
        return ESmartDialogueEqualOperation::EEO_Greater;
    }
    if (OperationString == "<")
    {
        return ESmartDialogueEqualOperation::EEO_Less;
    }
    if (OperationString == ">=")
    {
        return ESmartDialogueEqualOperation::EEO_GreaterOrEquals;
    }
    if (OperationString == "<=")
    {
        return ESmartDialogueEqualOperation::EEO_LessOrEquals;
    }
    
    UE_LOG(LogTemp, Error, TEXT("Invalid OperationString: %s"), *OperationString);
    return ESmartDialogueEqualOperation::EEO_Equals;
}

void SConditionsComboBoxList::RefreshList()
{
    ListBox->ClearChildren();

    GetOptions();

    for (int32 Index = 0; Index < InitialConditions.Num(); ++Index)
    {
        TSharedPtr<FString> SelectedVarItem = VariableOptions[0];
        TSharedPtr<FString> SelectedOperationItem = OperationOptions[0];

        for (auto Element : VariableOptions)
        {
            if (InitialConditions[Index].Key == *Element.Get())
            {
                SelectedVarItem = Element;
                break;
            }
        }
        for (auto Element : OperationOptions)
        {
            if (EnumOperationToString(InitialConditions[Index].EqualOperation) == *Element.Get())
            {
                SelectedOperationItem = Element;
                break;
            }
        }
        
        ListBox->AddSlot()
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            [
                SNew(SComboBox<TSharedPtr<FString>>)
                .OptionsSource(&VariableOptions)
                .OnSelectionChanged(this, &SConditionsComboBoxList::OnVariableSelectionChanged, Index)
                .InitiallySelectedItem(SelectedVarItem)
                .OnGenerateWidget(this, &SConditionsComboBoxList::MakeWidgetForVariableOption)
                .Content()
                [
                    SNew(STextBlock)
                    .Text_Lambda([this, Index] { return FText::FromString(InitialConditions[Index].Key); })
                ]
            ]
            + SHorizontalBox::Slot()
            [
                SNew(SComboBox<TSharedPtr<FString>>)
                .OptionsSource(&OperationOptions)
                .OnSelectionChanged(this, &SConditionsComboBoxList::OnOperationSelectionChanged, Index)
                .InitiallySelectedItem(SelectedOperationItem)
                .OnGenerateWidget(this, &SConditionsComboBoxList::MakeWidgetForOperationOption)
                .Content()
                [
                    SNew(STextBlock)
                    .Text_Lambda([this, Index] { return FText::FromString(EnumOperationToString(InitialConditions[Index].EqualOperation)); })
                ]
            ]
            + SHorizontalBox::Slot()
            [
                SNew(SEditableTextBox)
                .Text(FText::AsNumber(InitialConditions[Index].Value))
                .OnTextChanged(this, &SConditionsComboBoxList::OnValueTextChanged, Index)
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                SNew(SButton)
                .Text(FText::FromString("X"))
                .OnClicked(this, &SConditionsComboBoxList::OnDeleteButtonClicked, Index)
            ]
        ];
    }
}

void SConditionsComboBoxList::GetOptions()
{
    VariableOptions = Editor->GetAllVariablesList();
    OperationOptions = Editor->GetOperations(true);
}

TSharedRef<SWidget> SConditionsComboBoxList::MakeWidgetForVariableOption(TSharedPtr<FString> InOption)
{
    return SNew(STextBlock).Text(FText::FromString(*InOption));
}

TSharedRef<SWidget> SConditionsComboBoxList::MakeWidgetForOperationOption(TSharedPtr<FString> InOption)
{
    return SNew(STextBlock).Text(FText::FromString(*InOption));
}

FReply SConditionsComboBoxList::OnAddButtonClicked()
{
    FIf NewVar;
    NewVar.Key = *VariableOptions[0];
    NewVar.EqualOperation = ESmartDialogueEqualOperation::EEO_Equals;
    NewVar.Value = 0;

    InitialConditions.Add(NewVar);
    RefreshList();
    Editor->GetDialogue()->AddIfElement(Editor->GetSelectedBranchName(), NewVar);

    return FReply::Handled();
}

void SConditionsComboBoxList::OnVariableSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo, int32 Index)
{
    if (NewValue.IsValid())
    {
        InitialConditions[Index].Key = *NewValue.Get();
        Editor->GetDialogue()->UpdateIfElement(Editor->GetSelectedBranchName(), Index, InitialConditions[Index]);
    }
}

void SConditionsComboBoxList::OnOperationSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo, int32 Index)
{
    if (NewValue.IsValid())
    {
        InitialConditions[Index].EqualOperation = EnumOperationFromString(*NewValue.Get());
        Editor->GetDialogue()->UpdateIfElement(Editor->GetSelectedBranchName(), Index, InitialConditions[Index]);
    }
}

void SConditionsComboBoxList::OnValueTextChanged(const FText& NewValue, int32 Index)
{    
    InitialConditions[Index].Value = FCString::Atoi(*NewValue.ToString());
    Editor->GetDialogue()->UpdateIfElement(Editor->GetSelectedBranchName(), Index, InitialConditions[Index]);
}

FReply SConditionsComboBoxList::OnDeleteButtonClicked(int32 Index)
{
    if (InitialConditions.IsValidIndex(Index))
    {
        InitialConditions.RemoveAt(Index);
        RefreshList();
        Editor->GetDialogue()->RemoveIfElement(Editor->GetSelectedBranchName(), Index);
    }

    return FReply::Handled();
}

