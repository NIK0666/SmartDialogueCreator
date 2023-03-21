// SmartDialogueResourceEditorToolkit.h
#pragma once

#include "CoreMinimal.h"
#include "Toolkits/IToolkit.h"

class USmartDialogue;

class FSmartDialogueResourceEditorToolkit : public FAssetEditorToolkit
{
public:

    FSmartDialogueResourceEditorToolkit();
    virtual ~FSmartDialogueResourceEditorToolkit() override;

    

    
    virtual void Initialize(USmartDialogue* InDialogueResource, const EToolkitMode::Type InMode, const TSharedPtr<class IToolkitHost>& InToolkitHost);

    // IToolkit interface
    virtual FName GetToolkitFName() const override;
    virtual FText GetBaseToolkitName() const override;
    virtual FString GetWorldCentricTabPrefix() const override;
    virtual FLinearColor GetWorldCentricTabColorScale() const override;
    // End of IToolkit interface
};
