#include "SDialoguePlayerTab.h"

#include "DialogueManager.h"
#include "SAnswerButtonRow.h"
#include "SDialogueElementRow.h"
#include "Toolkit/FSmartDialogueEditor.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SWidgetSwitcher.h"


class SAnswerButtonRow;

void SDialoguePlayerTab::Construct(const FArguments& InArgs)
{
    Editor = InArgs._SmartDialogueEditor;
    ChildSlot
    [
        SNew(SVerticalBox)        
        + SVerticalBox::Slot()
        .FillHeight(1)
        [
            SAssignNew(DialogueListView, SListView<TSharedPtr<FDialogueElementData>>)
            .SelectionMode(ESelectionMode::None)
            .ItemHeight(64)
            .ListItemsSource(&DialogueElements)
            .OnGenerateRow(this, &SDialoguePlayerTab::GenerateDialogueRow)
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(4.f, 8.f)
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                SNew(SButton)
                .ButtonStyle(FAppStyle::Get(), "FlatButton")
                .OnClicked(this, &SDialoguePlayerTab::OnPlayClicked)
                .IsFocusable(false)
                .ContentPadding(FMargin(2, 2))
                [
                    SNew(SBox)
                    .WidthOverride(32)
                    .HeightOverride(32)
                    [
                        SNew(SImage)
                        .Image(FAppStyle::GetBrush("Icons.Toolbar.Play"))
                    ]
                ]
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                SNew(SButton)
                .ButtonStyle(FAppStyle::Get(), "FlatButton")
                .IsFocusable(false)
                .OnClicked(this, &SDialoguePlayerTab::OnStopClicked)
                .ContentPadding(FMargin(2, 2))
                [
                    SNew(SBox)
                    .WidthOverride(32)
                    .HeightOverride(32)
                    [
                        SNew(SImage)
                        .Image(FAppStyle::GetBrush("Icons.Toolbar.Stop"))
                    ]
                ]
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                SNew(SButton)
                .ButtonStyle(FAppStyle::Get(), "FlatButton")
                .IsFocusable(false)
                .OnClicked(this, &SDialoguePlayerTab::OnSwitchLocaleClicked)
                .ContentPadding(FMargin(2, 2))
                [
                    SNew(SBox)
                    .WidthOverride(32)
                    .HeightOverride(32)
                    [
                        SNew(SImage)
                        .Image(FAppStyle::GetBrush( "Icons.Settings"))
                    ]
                ]
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                SNew(SButton)
                .ButtonStyle(FAppStyle::Get(), "FlatButton")
                .IsFocusable(false)
                .OnClicked(this, &SDialoguePlayerTab::OnClearClicked)
                .ContentPadding(FMargin(2, 2))
                [
                    SNew(SBox)
                    .WidthOverride(32)
                    .HeightOverride(32)
                    [
                        SNew(SImage)
                        .Image(FAppStyle::GetBrush("Icons.Delete"))
                    ]
                ]
            ]
        ]
        // + SVerticalBox::Slot()
        // .AutoHeight()
        // [
        //     SAssignNew(NextPhraseButton, SButton)
        //    .HAlign(HAlign_Fill)
        //    .VAlign(VAlign_Fill)
        //    .Text(FText::FromString(TEXT("Next Phrase")))
        //    .OnClicked(this, &SDialoguePlayerTab::OnNextPhraseClicked)
        // ]
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SBox)
            .HeightOverride(300)
            [
                SAssignNew(AnswerSwitcher, SWidgetSwitcher)
                +SWidgetSwitcher::Slot()
                [
                    SAssignNew(AnswerScrollBox, SScrollBox)
                    .Style(FAppStyle::Get(), "ScrollBox")
                ]
                + SWidgetSwitcher::Slot()
                [
                    SAssignNew(NextPhraseButton, SButton)
                    .HAlign(HAlign_Fill)
                    .VAlign(VAlign_Fill)
                    .OnClicked(this, &SDialoguePlayerTab::OnNextPhraseClicked)
                ]
            ]
        ]
    ];
}

TSharedRef<ITableRow> SDialoguePlayerTab::GenerateDialogueRow(TSharedPtr<FDialogueElementData> InDialogueElementData, const TSharedRef<STableViewBase>& OwnerTable)
{
    return SNew(STableRow<TSharedPtr<FDialogueElementData>>, OwnerTable)
        [
            SNew(SDialogueElementRow)
            .DialogueElementData(*InDialogueElementData)
        ];
}

FReply SDialoguePlayerTab::OnPlayClicked()
{
    if (DialManager == nullptr)
    {
        DialManager = NewObject<UDialogueManager>();

        // Subscribe to delegates
        DialManager->OnShowBranchOptions.AddRaw(this, &SDialoguePlayerTab::OnShowBranchOptionsHandler);
        DialManager->OnHideBranchOptions.AddRaw(this, &SDialoguePlayerTab::OnHideBranchOptionsHandler);
        DialManager->OnShowPhrase.AddRaw(this, &SDialoguePlayerTab::OnShowPhraseHandler);
        DialManager->OnCloseDialogue.AddRaw(this, &SDialoguePlayerTab::OnCloseDialogueHandler);
        DialManager->OnEventTriggered.AddRaw(this, &SDialoguePlayerTab::OnEventTriggeredHandler);
        DialManager->OnVariableChanged.AddRaw(this, &SDialoguePlayerTab::OnVariableChanged);
    }
    
    if (!DialManager->IsDialogueProgressInitialized())
    {
        DialManager->InitializeDialogueProgress(Editor->GetDialogueConfig());    
    }
    
    AnswerSwitcher->SetActiveWidgetIndex(1);    
    DialManager->StartDialogue(Editor->GetDialogue());
    
    return FReply::Handled();
}

FReply SDialoguePlayerTab::OnClearClicked()
{
    DialogueElements.Empty();
    DialogueListView->RequestListRefresh();

    AnswerScrollBox->ClearChildren();
    AnswerSwitcher->SetActiveWidgetIndex(0);
    
    return FReply::Handled();
}

// Event handlers
void SDialoguePlayerTab::OnShowBranchOptionsHandler(const TArray<FText>& BranchTexts, const TArray<int32>& InBranchIndices)
{
    UpdateAnswers(BranchTexts);
    BranchIndices = InBranchIndices;
    
    UE_LOG(LogTemp, Log, TEXT("OnShowBranchOptions:"));
    for (const FText& Text : BranchTexts)
    {
        UE_LOG(LogTemp, Log, TEXT("%s"), *Text.ToString());
    }
}

void SDialoguePlayerTab::OnHideBranchOptionsHandler()
{
    UpdateAnswers({});
    BranchIndices.Empty();
}


void SDialoguePlayerTab::OnShowPhraseHandler(const FText& PhraseText, const FString& SpeakerId)
{
    UE_LOG(LogTemp, Log, TEXT("OnShowPhrase: %s (Speaker: %s)"), *PhraseText.ToString(), *SpeakerId);

    FDialogueElementData NewElementData;
    NewElementData.Title = SpeakerId;
    NewElementData.Content = PhraseText;
    NewElementData.TitleColor = (SpeakerId == HeroName) ? FLinearColor(0.5f, 0.5f, 0) : FLinearColor(0, 0.5f, 0.5f);
    NewElementData.ContentColor = (SpeakerId == HeroName) ? FLinearColor::White : FLinearColor(0.8f, 0.8f, 0.8f);
    NewElementData.bIsTitleVisible = DialogueElements.Num() == 0 || DialogueElements.Last()->Title != SpeakerId;

    DialogueElements.Add(MakeShared<FDialogueElementData>(NewElementData));

    DialogueListView->RequestListRefresh();
    DialogueListView->ScrollToBottom();
}

void SDialoguePlayerTab::OnCloseDialogueHandler()
{
    UE_LOG(LogTemp, Log, TEXT("OnCloseDialogue"));

    FDialogueElementData NewElementData;
    NewElementData.Content = FText::FromString(TEXT("ДИАЛОГ ЗАКОНЧЕН\n"));
    NewElementData.ContentColor = FLinearColor::Yellow;
    NewElementData.bIsTitleVisible = false;

    DialogueElements.Add(MakeShared<FDialogueElementData>(NewElementData));

    DialogueListView->RequestListRefresh();
    DialogueListView->ScrollToBottom();

    AnswerSwitcher->SetActiveWidgetIndex(0);
}

void SDialoguePlayerTab::OnEventTriggeredHandler(const FString& EventName, const FString& EventParam)
{
    UE_LOG(LogTemp, Log, TEXT("OnEventTriggered: %s (Param: %s)"), *EventName, *EventParam);

    FDialogueElementData NewElementData;
    NewElementData.Title = FString::Printf(TEXT("Event: %s (Param: %s)"), *EventName, *EventParam);
    NewElementData.TitleColor = FLinearColor::Red;
    NewElementData.bIsContentVisible = false;

    DialogueElements.Add(MakeShared<FDialogueElementData>(NewElementData));

    DialogueListView->RequestListRefresh();
    DialogueListView->ScrollToBottom();
}

void SDialoguePlayerTab::OnVariableChanged(bool bisPublic, const FString& VarName, int OldValue, int NewValue)
{
    UE_LOG(LogTemp, Log, TEXT("%s Variable Changed: %s (From: %d to %d)"), bisPublic ? *FString(TEXT("Public")) : *FString(TEXT("Local")), *VarName, OldValue, NewValue);

    FDialogueElementData NewElementData;
    NewElementData.Title = FString::Printf(TEXT("%s Variable Changed: %s"), bisPublic ? *FString(TEXT("Public")) : *FString(TEXT("Local")), *VarName);
    NewElementData.TitleColor = FLinearColor::Gray;
    NewElementData.Content = FText::FromString(FString::Printf(TEXT("From: %d to %d"), OldValue, NewValue));
    NewElementData.ContentColor = FLinearColor::Gray;
    
    DialogueElements.Add(MakeShared<FDialogueElementData>(NewElementData));

    DialogueListView->RequestListRefresh();
    DialogueListView->ScrollToBottom();
}

FReply SDialoguePlayerTab::OnNextPhraseClicked()
{
    DialManager->ShowNextPhrase();
    
    return FReply::Handled();
}

FReply SDialoguePlayerTab::OnStopClicked()
{
    DialManager->InitializeDialogueProgress(Editor->GetDialogueConfig());
    return FReply::Handled();
}

FReply SDialoguePlayerTab::OnSwitchLocaleClicked()
{
    return FReply::Handled();
}

void SDialoguePlayerTab::UpdateAnswers(const TArray<FText>& Answers)
{
    AnswerScrollBox->ClearChildren();

    for (int32 i = 0; i < Answers.Num(); i++)
    {
        AnswerScrollBox->AddSlot()
        [
            SNew(SAnswerButtonRow)
            .AnswerText(Answers[i])
            .AnswerIndex(i)
            .OnSelectedAnswer(this, &SDialoguePlayerTab::OnAnswerSelected)
        ];
    }

    AnswerSwitcher->SetActiveWidgetIndex(Answers.Num() > 0 ? 0 : 1);

}


void SDialoguePlayerTab::OnAnswerSelected(int32 AnswerIndex)
{
    DialManager->SelectBranch(AnswerIndex);
}

FReply SDialoguePlayerTab::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == EKeys::SpaceBar)
    {
        return OnStopClicked();
    }
    return FReply::Unhandled();
}