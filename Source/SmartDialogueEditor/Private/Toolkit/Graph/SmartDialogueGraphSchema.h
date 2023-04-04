// SmartDialogueGraphSchema.h

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "SmartDialogueGraphSchema.generated.h"

class UToolMenu;
class UGraphNodeContextMenuContext;

UCLASS()
class SMARTDIALOGUEEDITOR_API USmartDialogueGraphSchema : public UEdGraphSchema_K2
{
	GENERATED_BODY()

public:

	// Override required methods
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual TSharedPtr<FEdGraphSchemaAction> GetCreateCommentAction() const override;
	virtual void GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;

	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
	virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const override;

};