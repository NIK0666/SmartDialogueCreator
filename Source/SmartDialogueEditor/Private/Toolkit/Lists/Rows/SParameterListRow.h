// SParameterListRow.h

#pragma once

#include "CoreMinimal.h"
#include "SmartDialogueData.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
DECLARE_DELEGATE_OneParam(FParameterDataChanged, const FCustomParameterData& UpdatedParameter)


class SParameterListRow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SParameterListRow) {}
	SLATE_ATTRIBUTE(FString, Parameter)
	SLATE_ATTRIBUTE(FString, Desc)
	SLATE_EVENT(FParameterDataChanged, OnChanged)
	SLATE_EVENT(FSimpleDelegate, OnDeleteButtonClicked)
SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
	FSimpleDelegate OnDeleteButtonClicked;
	FParameterDataChanged OnChanged;
private:
	TAttribute<FString> Parameter;
	TAttribute<FString> Desc;
	
	TSharedPtr<SEditableTextBox> DescriptionEditableTextBox;
	TSharedPtr<SEditableTextBox> ParameterEditableTextBox;


};