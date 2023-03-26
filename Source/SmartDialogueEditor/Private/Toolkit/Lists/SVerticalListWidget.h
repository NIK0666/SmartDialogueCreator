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
	
	void OnRemoveButtonClicked(int32 IndexToRemove);
	FReply OnAddButtonClicked();
	TSharedRef<SWidget> GenerateStringItemWidget(TSharedPtr<FString> InItem);
	void OnComboBoxSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	
protected:
	virtual TSharedRef<SWidget> GetItemContent(const FString& Item);
	virtual TArray< TSharedPtr<FString>> GetAllStrings();
	virtual void OnSelected(const FString& SelectedItem);
	
	FText Title;

private:
	TSharedPtr<SVerticalBox> ListContainer;
	TArray<FString> Data;
};
