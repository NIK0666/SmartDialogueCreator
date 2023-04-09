// SDialConfigWidget.h

#pragma once

#include "CoreMinimal.h"
#include "SmartDialConfig.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SCharacterComboBox;
class FSmartDialogueEditor;

class SDialConfigWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDialConfigWidget) {}
	SLATE_ARGUMENT(FSmartDialogueEditor*, SmartDialogueEditor)
	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);

	void OnPublicVarDeleted(TSharedPtr<FString> VarId);
	void OnLocalVarDeleted(TSharedPtr<FString> VarId);
	
	void UpdateData();

private:
	
	void OnCharacterSelected(TSharedPtr<FString> NewSelection);
	FReply OnAddButtonClicked();
	FReply OnAddParameterClicked();
	void OnDeleteButtonClicked(TSharedPtr<FString> CharacterId);	
	void AddCharacterRow(FString Id = "", FText Name = FText::GetEmpty());
	void AddGlobalVarRow(const FString& Key = "", const int32& Value = 0, const FString& Desc = "");
	void AddLocalVarRow(const FString& Key, const int32& Value, const FString& Desc);
	
	void AddParameterRow(const FString& Key = "", const FString& Desc = "");

	FReply OnAddPublicVarClicked();

	FReply OnAddLocalVarClicked();


	FReply OnTabButtonClicked(int32 TabIndex);
	void UpdateButtonStyles();
	void AddTabButton(const FString& ButtonText);

	void AddCharactersTab(TSharedPtr<SVerticalBox> VerticalBoxContent);
	void AddGlobalVarsTab(TSharedPtr<SVerticalBox> VerticalBoxContent);
	void AddLocalVarsTab(TSharedPtr<SVerticalBox> VerticalBoxContent);
	void AddParametersTab(TSharedPtr<SVerticalBox> VerticalBoxContent);

	TSharedPtr<SVerticalBox> ScrollBoxGlobalVarsContent;
	TArray<TSharedPtr<FString>> PublicVarIds;
	int32 PublicVarCounter = 0;

	TSharedPtr<SVerticalBox> ScrollBoxLocalVarsContent;
	TArray<TSharedPtr<FString>> LocalVarIds;
	int32 LocalVarCounter = 0;
	
	TSharedPtr<SVerticalBox> ScrollBoxContent;
	
	TSharedPtr<SVerticalBox> ParametersScrollBoxContent;

	TSharedPtr<SWidgetSwitcher> WidgetSwitcher;
	int32 ActiveTabIndex = 0;
	TArray<TSharedPtr<SButton>> TabButtons;
	
	USmartDialConfig* DialConfig = nullptr;

	TSharedPtr<SCharacterComboBox> CharacterComboBox;
	TSharedPtr<STextBlock> CharacterTextBlock;
	TArray<TSharedPtr<FString>> CharacterList;
	
	FSmartDialogueEditor* SmartDialogueEditor = nullptr;
};
