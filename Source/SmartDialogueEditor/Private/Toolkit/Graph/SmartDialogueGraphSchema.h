// SmartDialogueGraphSchema.h

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "SmartDialogueGraphSchema.generated.h"

UCLASS()
class SMARTDIALOGUEEDITOR_API USmartDialogueGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:

	// Override required methods
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual TSharedPtr<FEdGraphSchemaAction> GetCreateCommentAction() const override;
	virtual void GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
};