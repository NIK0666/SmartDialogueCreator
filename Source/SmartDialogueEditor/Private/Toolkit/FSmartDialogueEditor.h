// 

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"

class USmartDialogue;

class SMARTDIALOGUEEDITOR_API FSmartDialogueEditor final : public FAssetEditorToolkit
{
public:
	virtual FName GetToolkitFName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

private:
	USmartDialogue* EditingObject;
};
