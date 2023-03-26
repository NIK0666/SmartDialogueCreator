// SVerticalListWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SVerticalListWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SVerticalListWidget)
		: _Title(FText::GetEmpty())
	{}
	SLATE_ARGUMENT(FText, Title)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void UpdateData(const TArray<FString>& NewData);
	TArray< TSharedPtr< FString > > GetAllStrings();
	void OnSelected(const FString& SelectedItem);
	FReply OnChangeButtonClicked();
	FReply OnRemoveButtonClicked(int32 IndexToRemove);
	FReply OnAddButtonClicked();
	TSharedRef<SWidget> GenerateStringItemWidget(TSharedPtr<FString> InItem);
	void OnComboBoxSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	void ShowSelectionMenu();

private:
	TSharedPtr<SVerticalBox> ListContainer;
	FText Title;
	TArray<FString> Data;
};
