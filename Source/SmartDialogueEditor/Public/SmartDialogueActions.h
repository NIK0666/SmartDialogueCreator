// SmartDialogueActions.h
#pragma once

#include "AssetTypeActions_Base.h"

class FSmartDialogueActions : public FAssetTypeActions_Base
{
public:

	FSmartDialogueActions(EAssetTypeCategories::Type InCategory);
	
	// Override FAssetTypeActions_Base
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type DialogueCategoryBit;
};