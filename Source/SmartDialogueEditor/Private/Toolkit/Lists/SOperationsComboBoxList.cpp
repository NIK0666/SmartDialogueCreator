#include "SOperationsComboBoxList.h"

#include "SmartDialogue.h"
#include "Helpers/EditorDataHelper.h"

void SOperationsComboBoxList::Construct(const FArguments& InArgs)
{
    InitialVars = InArgs._InitialVars;
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
                .OnClicked(this, &SOperationsComboBoxList::OnAddButtonClicked)
            ]
        ]
        + SVerticalBox::Slot()
        [
            SAssignNew(ListBox, SVerticalBox)
        ]
    ];

    RefreshList();
}

void SOperationsComboBoxList::UpdateInitialVars(const TArray<FSmartDialogueVars>& NewInitialVars)
{
    InitialVars = NewInitialVars;
    RefreshList();
}

FString SOperationsComboBoxList::ESmartDialogueOperationToString(ESmartDialogueOperation Operation)
{
    switch (Operation)
    {
    case ESmartDialogueOperation::EO_Equals:
        return "=";
    case ESmartDialogueOperation::EO_Divide:
        return "/";
    case ESmartDialogueOperation::EO_Minus:
        return "-";
    case ESmartDialogueOperation::EO_Multiply:
        return "*";
    case ESmartDialogueOperation::EO_Plus:
        return "+";
    }
    
    return "";
}

ESmartDialogueOperation SOperationsComboBoxList::ESmartDialogueOperationFromString(FString& OperationString)
{
    if (OperationString == "=")
    {
        return ESmartDialogueOperation::EO_Equals;
    }
    if (OperationString == "+")
    {
        return ESmartDialogueOperation::EO_Plus;
    }
    if (OperationString == "-")
    {
        return ESmartDialogueOperation::EO_Minus;
    }
    if (OperationString == "*")
    {
        return ESmartDialogueOperation::EO_Multiply;
    }
    if (OperationString == "/")
    {
        return ESmartDialogueOperation::EO_Divide;
    }
    
    UE_LOG(LogTemp, Error, TEXT("Invalid OperationString: %s"), *OperationString);
    return ESmartDialogueOperation::EO_Equals;
}

void SOperationsComboBoxList::RefreshList()
{
    ListBox->ClearChildren();

    GetOptions();

    for (int32 Index = 0; Index < InitialVars.Num(); ++Index)
    {
        TSharedPtr<FString> SelectedVarItem = VariableOptions[0];
        TSharedPtr<FString> SelectedOperationItem = OperationOptions[0];

        for (auto Element : VariableOptions)
        {
            if (InitialVars[Index].Key == *Element.Get())
            {
                SelectedVarItem = Element;
                break;
            }
        }
        for (auto Op : OperationOptions)
        {
            if (Op->Equals(ESmartDialogueOperationToString(InitialVars[Index].Operation)))
            {
                SelectedOperationItem = Op;
                break;
            }
        }
        
        ListBox->AddSlot()
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .FillWidth(1.f)
            [
                SNew(SComboBox<TSharedPtr<FString>>)
                .OptionsSource(&VariableOptions)
                .OnSelectionChanged(this, &SOperationsComboBoxList::OnVariableSelectionChanged, Index)
                .InitiallySelectedItem(SelectedVarItem)
                .OnGenerateWidget(this, &SOperationsComboBoxList::MakeWidgetForVariableOption)
                .Content()
                [
                    SNew(STextBlock)
                    .Text_Lambda([this, Index] { return FText::FromString(InitialVars[Index].Key); })
                ]
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                SNew(SComboBox<TSharedPtr<FString>>)
                .OptionsSource(&OperationOptions)
                .OnSelectionChanged(this, &SOperationsComboBoxList::OnOperationSelectionChanged, Index)
                .InitiallySelectedItem(SelectedOperationItem)
                .OnGenerateWidget(this, &SOperationsComboBoxList::MakeWidgetForOperationOption)
                .Content()
                [
                    SNew(STextBlock)
                    .Text_Lambda([this, Index] { return FText::FromString(ESmartDialogueOperationToString(InitialVars[Index].Operation)); })
                ]
            ]
            .AutoWidth()
            + SHorizontalBox::Slot()
            [
                SNew(SEditableTextBox)
                .Text(FText::AsNumber(InitialVars[Index].Value))
                .OnTextChanged(this, &SOperationsComboBoxList::OnValueTextChanged, Index)
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                SNew(SButton)
                .Text(FText::FromString("X"))
                .OnClicked(this, &SOperationsComboBoxList::OnDeleteButtonClicked, Index)
            ]
        ];
    }
}

void SOperationsComboBoxList::GetOptions()
{
    VariableOptions = Editor->GetAllVariablesList();
    OperationOptions = Editor->GetOperations(true);
}

TSharedRef<SWidget> SOperationsComboBoxList::MakeWidgetForVariableOption(TSharedPtr<FString> InOption)
{
    return SNew(STextBlock).Text(FText::FromString(*InOption));
}

TSharedRef<SWidget> SOperationsComboBoxList::MakeWidgetForOperationOption(TSharedPtr<FString> InOption)
{
    return SNew(STextBlock).Text(FText::FromString(*InOption));
}

FReply SOperationsComboBoxList::OnAddButtonClicked()
{
    FSmartDialogueVars NewVar;
    NewVar.Key = *VariableOptions[0];
    NewVar.Operation = ESmartDialogueOperation::EO_Equals;
    NewVar.Value = 0;

    InitialVars.Add(NewVar);
    RefreshList();
    UEditorDataHelper::AddVarElement(Editor, Editor->GetSelectedBranchName(), NewVar);

    return FReply::Handled();
}

void SOperationsComboBoxList::OnVariableSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo, int32 Index)
{
    if (NewValue.IsValid())
    {
        InitialVars[Index].Key = *NewValue.Get();
        UEditorDataHelper::UpdateVarElement(Editor, Editor->GetSelectedBranchName(), Index, InitialVars[Index]);
    }
}

void SOperationsComboBoxList::OnOperationSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo, int32 Index)
{
    if (NewValue.IsValid())
    {
        InitialVars[Index].Operation = ESmartDialogueOperationFromString(*NewValue.Get());
        UEditorDataHelper::UpdateVarElement(Editor, Editor->GetSelectedBranchName(), Index, InitialVars[Index]);
    }
}

void SOperationsComboBoxList::OnValueTextChanged(const FText& NewValue, int32 Index)
{    
    InitialVars[Index].Value = FCString::Atoi(*NewValue.ToString());
    UEditorDataHelper::UpdateVarElement(Editor, Editor->GetSelectedBranchName(), Index, InitialVars[Index]);
}

FReply SOperationsComboBoxList::OnDeleteButtonClicked(int32 Index)
{
    if (InitialVars.IsValidIndex(Index))
    {
        InitialVars.RemoveAt(Index);
        RefreshList();
        UEditorDataHelper::RemoveVarElement(Editor, Editor->GetSelectedBranchName(), Index);
    }

    return FReply::Handled();
}

