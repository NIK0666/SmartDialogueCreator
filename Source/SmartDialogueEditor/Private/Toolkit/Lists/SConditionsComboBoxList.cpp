// 

#include "SConditionsComboBoxList.h"
#include "SmartDialogue.h"
#include "Helpers/EditorDataHelper.h"

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
            if (ESmartDialogueEqualOperationHelper::EnumOperationToString(InitialConditions[Index].EqualOperation) == *Element.Get())
            {
                SelectedOperationItem = Element;
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
            .AutoWidth()
            [
                SNew(SComboBox<TSharedPtr<FString>>)
                .OptionsSource(&OperationOptions)
                .OnSelectionChanged(this, &SConditionsComboBoxList::OnOperationSelectionChanged, Index)
                .InitiallySelectedItem(SelectedOperationItem)
                .OnGenerateWidget(this, &SConditionsComboBoxList::MakeWidgetForOperationOption)
                .Content()
                [
                    SNew(STextBlock)
                    .Text_Lambda([this, Index] { return FText::FromString(ESmartDialogueEqualOperationHelper::EnumOperationToString(InitialConditions[Index].EqualOperation)); })
                ]
            ]
            + SHorizontalBox::Slot()
			.AutoWidth()
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
    UEditorDataHelper::AddIfElement(Editor, Editor->GetSelectedBranchName(), NewVar);

    return FReply::Handled();
}

void SConditionsComboBoxList::OnVariableSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo, int32 Index)
{
    if (NewValue.IsValid())
    {
        InitialConditions[Index].Key = *NewValue.Get();
        UEditorDataHelper::UpdateIfElement(Editor, Editor->GetSelectedBranchName(), Index, InitialConditions[Index]);
    }
}

void SConditionsComboBoxList::OnOperationSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo, int32 Index)
{
    if (NewValue.IsValid())
    {
        InitialConditions[Index].EqualOperation = ESmartDialogueEqualOperationHelper::EnumOperationFromString(*NewValue.Get());
        UEditorDataHelper::UpdateIfElement(Editor, Editor->GetSelectedBranchName(), Index, InitialConditions[Index]);
    }
}

void SConditionsComboBoxList::OnValueTextChanged(const FText& NewValue, int32 Index)
{    
    InitialConditions[Index].Value = FCString::Atoi(*NewValue.ToString());
    UEditorDataHelper::UpdateIfElement(Editor, Editor->GetSelectedBranchName(), Index, InitialConditions[Index]);
}

FReply SConditionsComboBoxList::OnDeleteButtonClicked(int32 Index)
{
    if (InitialConditions.IsValidIndex(Index))
    {
        InitialConditions.RemoveAt(Index);
        RefreshList();
        UEditorDataHelper::RemoveIfElement(Editor, Editor->GetSelectedBranchName(), Index);
    }

    return FReply::Handled();
}

