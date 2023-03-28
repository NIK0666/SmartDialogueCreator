// 


#include "AssetTypeActions_SmartDialConfig.h"
#include "SmartDialConfig.h"
#include "IAssetTools.h"

FAssetTypeActions_SmartDialConfig::FAssetTypeActions_SmartDialConfig(EAssetTypeCategories::Type InCategory): DialogueCategoryBit(InCategory)
{
}

FText FAssetTypeActions_SmartDialConfig::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_SmartDialConfig", "Dialogues Config");
}

UClass* FAssetTypeActions_SmartDialConfig::GetSupportedClass() const
{
	return USmartDialConfig::StaticClass();
}

FColor FAssetTypeActions_SmartDialConfig::GetTypeColor() const
{
	return FColor::Purple;
}

uint32 FAssetTypeActions_SmartDialConfig::GetCategories()
{
	return DialogueCategoryBit;
}

void FAssetTypeActions_SmartDialConfig::OpenAssetEditor(const TArray<UObject*>& InObjects,
                                                        TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	// Создайте окно редактора параметров для каждого UObject
	for (UObject* Object : InObjects)
	{
		if (Object != nullptr)
		{
			FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
			// Создайте параметры просмотра свойств
	
			FDetailsViewArgs DetailsViewArgs;
			DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
			DetailsViewArgs.bHideSelectionTip = true;
			DetailsViewArgs.bUpdatesFromSelection = false;
			DetailsViewArgs.bLockable = false;
			DetailsViewArgs.bAllowSearch = true;
			DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
			DetailsViewArgs.bHideSelectionTip = true;			
			
			TSharedRef<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
			DetailsView->SetObject(Object);
	
			// Откройте окно с редактором параметров
			TSharedPtr<SWindow> Window = SNew(SWindow)
					.Title(FText::FromString(Object->GetName()))
					.ClientSize(FVector2D(800, 600))
					.SupportsMaximize(true)
					.SupportsMinimize(true)
			[
				DetailsView
			];
	
			// Добавьте окно в группу окон редактора
			if (FSlateApplication::IsInitialized())
			{
				FSlateApplication::Get().AddWindow(Window.ToSharedRef());
			}
		}
	}
}
