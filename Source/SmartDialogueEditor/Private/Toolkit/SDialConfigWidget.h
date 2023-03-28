// SDialConfigWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SDialConfigWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDialConfigWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SComboBox<TSharedPtr<FString>>> CharacterComboBox;
	TSharedPtr<SVerticalBox> ScrollBoxContent;

	TArray<TSharedPtr<FString>> CharacterList;
	TSharedPtr<SWidgetSwitcher> TabSwitcher;


	void OnCharacterSelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);

	FReply OnAddButtonClicked();
	void OnDeleteButtonClicked(TSharedPtr<FString> CharacterId);
	
	void AddCharacterRow(FString Id = "", FString Name = "");
};