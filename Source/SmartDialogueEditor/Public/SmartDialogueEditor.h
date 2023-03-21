#pragma once

#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "Modules/ModuleManager.h"

class IAssetTypeActions;
class FAssetTypeActions_Base;

class FSmartDialogueEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
private:    
    TArray< TSharedPtr<IAssetTypeActions>> RegisteredAssetTypeActions;
    EAssetTypeCategories::Type SmartDialogueCategoryBit;
};