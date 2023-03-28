// AssetTypeActions_SmartDialConfig.h

#pragma once

#include "AssetTypeActions_Base.h"

class FAssetTypeActions_SmartDialConfig : public FAssetTypeActions_Base
{
public:
	FAssetTypeActions_SmartDialConfig(EAssetTypeCategories::Type InCategory);
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;

	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;

private:
	EAssetTypeCategories::Type DialogueCategoryBit;
};