// AssetTypeActions_SmartDialogue.h
#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "AssetTypeActions_Base.h"

class FAssetTypeActions_SmartDialogue : public FAssetTypeActions_Base
{
public:

	FAssetTypeActions_SmartDialogue(EAssetTypeCategories::Type InCategory);
	
	// Override FAssetTypeActions_Base
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;

	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) override; 


private:
	EAssetTypeCategories::Type DialogueCategoryBit;
};