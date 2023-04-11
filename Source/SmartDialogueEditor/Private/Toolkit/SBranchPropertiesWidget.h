// SBranchPropertiesWidget.h

#pragma once

#include "SmartDialogueData.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

class SHideBranchesComboBoxList;
class SShowBranchesComboBoxList;
class SOperationsComboBoxList;
class SConditionsComboBoxList;

class FSmartDialogueEditor;
class SBranchComboBox;

class SBranchPropertiesWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SBranchPropertiesWidget) {}
	SLATE_ARGUMENT(FSmartDialogueEditor*, SmartDialogueEditor)
	SLATE_END_ARGS()
	void Construct(const FArguments& InArgs);

private:
	FSmartDialogueEditor* SmartDialogueEditor;


	FSmartDialogueBranch* GetBranchDataPtr();
	// TArray<TSharedPtr<FString>> AllBranchesList;

	TSharedRef<SWidget> GetContentWidget();

	void OnBranchSelected(FSmartDialogueBranch& SmartDialogueBranch);
	void OnResetSelectedBranch();
	void OnBranchItemRemoved(FName& Name);

	void OnShowBranchRemoved(FName BranchName, int32 Index, FString Value);
	void OnShowBranchAdded(FName BranchName, FString String);
	void OnHideBranchRemoved(FName BranchName, int32 Index, FString Value);
	void OnHideBranchAdded(FName BranchName, FString String);
	void OnBranchRenamed(FName OldName, FName NewName);
	void UpdateWidgets();
	EVisibility GetEventParamVisibility() const;

	// Добавляем указатели на виджеты интерфейса
	TSharedPtr<SCheckBox> HiddenCheckBox;
	TSharedPtr<SCheckBox> HideSelfCheckBox;
	TSharedPtr<SCheckBox> ClosedCheckBox;
	TSharedPtr<SCheckBox> ShowChoiceCheckBox;
	TSharedPtr<SCheckBox> QueueCheckBox;
	TSharedPtr<SCheckBox> CheckAsORCheckBox;
	TSharedPtr<SBranchComboBox> StartBranchComboBox;
	TSharedPtr<SEditableTextBox> EventNameTextBox;
	TSharedPtr<SEditableTextBox> EventParamTextBox;
	TSharedPtr<SCheckBox> AfterBranchCheckBox;
	TSharedPtr<SWidget> ContentWidget;
	TSharedPtr<SVerticalBox> PropertiesContentWidget;

	TSharedPtr<SConditionsComboBoxList> CheckEntryConditionsList;
	TSharedPtr<SOperationsComboBoxList> ModifyVariablesList;
	TSharedPtr<SShowBranchesComboBoxList> ShowBranchesList;
	TSharedPtr<SHideBranchesComboBoxList> HideBranchesList;

};
