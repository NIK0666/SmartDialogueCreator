// 

#pragma once

#include "Toolkit/FSmartDialogueEditor.h"

class SHideBranchesComboBoxList : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SHideBranchesComboBoxList) {}
    SLATE_ARGUMENT(TArray<FString>, InitialStrings)
    SLATE_ARGUMENT(FSmartDialogueEditor*, Editor)
    SLATE_ARGUMENT(FText, Title)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    void UpdateInitialStrings(const TArray<FString>& NewInitialStrings);

private:
    TArray<FString> InitialStrings;
    TSharedPtr<SVerticalBox> ListBox;

    void OnComboBoxOpeningHandler();
    void RefreshList();

    void GetOptions() ;

    TSharedRef<SWidget> MakeWidgetForOption(TSharedPtr<FString> InOption);

    FReply OnAddButtonClicked();

    void OnSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo, int32 Index);

    FReply OnDeleteButtonClicked(int32 Index);
    
    TArray<TSharedPtr<FString>> Options;

    FSmartDialogueEditor* Editor;
};
