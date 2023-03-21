#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAssetTypeActions_Base;

class FSmartDialogueEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
private:    
    TArray<TSharedPtr<FAssetTypeActions_Base>> RegisteredAssetTypeActions;
};