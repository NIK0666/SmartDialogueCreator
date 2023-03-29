// SBranchPropertiesWidget.h

#pragma once

#include "SmartDialogueData.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

class FSmartDialogueEditor;

class SBranchPropertiesWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBranchPropertiesWidget) {}
	SLATE_ARGUMENT(TSharedPtr<FSmartDialogueEditor>, SmartDialogueEditor)
	SLATE_ARGUMENT(FSmartDialogueBranch, DialogueBranchData)
	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<FSmartDialogueEditor> SmartDialogueEditor;
	FSmartDialogueBranch DialogueBranchData;
	
	TArray<TSharedPtr<FString>> AllBranchesList;

	void OnBranchSelected(FSmartDialogueBranch& SmartDialogueBranch);
	void UpdateWidgets();

	// Добавляем указатели на виджеты интерфейса
	TSharedPtr<SCheckBox> HiddenCheckBox;
	TSharedPtr<SCheckBox> HideSelfCheckBox;
	TSharedPtr<SCheckBox> ClosedCheckBox;
	TSharedPtr<SCheckBox> ShowChoiceCheckBox;
	TSharedPtr<SCheckBox> CheckAsORCheckBox;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> StartBranchComboBox;
	TSharedPtr<SEditableTextBox> EventNameTextBox;

};
