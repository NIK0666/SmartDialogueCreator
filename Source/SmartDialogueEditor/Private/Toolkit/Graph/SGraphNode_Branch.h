// SGraphNode_Branch.h
#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class UBranchNode;
class SWidgetSwitcher;
class SBranchPin;

class SMARTDIALOGUEEDITOR_API SGraphNode_Branch : public SGraphNode
{
public:
	DECLARE_DELEGATE_OneParam(FOnNodeTitleChanged, const FText&)
	

	SLATE_BEGIN_ARGS(SGraphNode_Branch) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UBranchNode* InNode);
	virtual void CreatePinWidgets() override;

protected:	
	virtual TSharedRef<SWidget> CreateNodeContentArea() override;
	virtual TSharedRef<SWidget> CreateTitleWidget(TSharedPtr<SNodeTitle> NodeTitle) override;
	virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;

	TSharedRef<SWidget> CreateNodeTitleWidget();
	FText GetNodeTitle() const;

	FOnNodeTitleChanged OnNodeTitleChanged;
	TSharedPtr<SWidgetSwitcher> TitleSwitcher;
	TSharedPtr<SEditableText> TitleEditableText;

	float MaxNodeWidth = 200.f;

};
