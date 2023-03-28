// SDialConfigWidget.h

#pragma once

#include "CoreMinimal.h"
#include "SmartDialConfig.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SDialConfigWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDialConfigWidget) {}
	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);

	void OnPublicVarDeleted(TSharedPtr<FString> VarId);
	void OnLocalVarDeleted(TSharedPtr<FString> VarId);

private:
	
	void OnCharacterSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	FReply OnAddButtonClicked();
	void OnDeleteButtonClicked(TSharedPtr<FString> CharacterId);	
	void AddCharacterRow(FString Id = "", FString Name = "");
	void AddGlobalVarRow(const FString& Key = "", const FString& Value = "", const FString& Desc = "");
	
	FReply OnAddPublicVarClicked();

	FReply OnAddLocalVarClicked();


	FReply OnTabButtonClicked(int32 TabIndex);
	void UpdateButtonStyles();
	void AddTabButton(const FString& ButtonText);
	
	void UpdateData();

	TSharedPtr<SVerticalBox> ScrollBoxGlobalVarsContent;
	TArray<TSharedPtr<FString>> PublicVarIds;
	int32 PublicVarCounter = 0;

	TSharedPtr<SVerticalBox> ScrollBoxLocalVarsContent;
	TArray<TSharedPtr<FString>> LocalVarIds;
	int32 LocalVarCounter = 0;
	
	TSharedPtr<SComboBox<TSharedPtr<FString>>> CharacterComboBox;
	TSharedPtr<SVerticalBox> ScrollBoxContent;

	TArray<TSharedPtr<FString>> CharacterList;
	TSharedPtr<SWidgetSwitcher> WidgetSwitcher;
	int32 ActiveTabIndex = 0;
	TArray<TSharedPtr<SButton>> TabButtons;
	
	USmartDialConfig* DialConfig = nullptr;
};
