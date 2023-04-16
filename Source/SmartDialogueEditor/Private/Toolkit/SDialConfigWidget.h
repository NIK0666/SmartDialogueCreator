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
	
	void UpdateData(bool bCharacters = true, bool bGlobalVars = true, bool bLocalVars = true, bool bParameters = true);

private:
	
	void OnCharacterSelected(TSharedPtr<FString> NewSelection);
	FReply OnAddCharacterButtonClicked();
	FReply OnAddParameterClicked();

	void OnCharacterChanged(const FCharacterData& CharacterData, int32 RowIndex);
	void OnCustomParameterChanged(const FCustomParameterData& CustomParameterData, int32 RowIndex);
	void OnGlobalVarChanged(const FVariableData& VariableData, int32 RowIndex);
	void OnLocalVarChanged(const FVariableData& VariableData, int32 RowIndex);

	void AddCharacterRow(FString Id = "", FText Name = FText::GetEmpty());
	void AddGlobalVarRow(const FString& Key = "", const int32& Value = 0, const FString& Desc = "");
	void AddLocalVarRow(const FString& Key = "", const int32& Value = 0, const FString& Desc = "");
	void AddParameterRow(const FString& Key = "", const FString& Desc = "");

	FReply OnAddPublicVarClicked();

	FReply OnAddLocalVarClicked();


	FReply OnTabButtonClicked(int32 TabIndex);
	void UpdateButtonStyles();
	void AddTabButton(const FString& ButtonText);

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
