// 


#include "SmartDialConfigFactory.h"

#include "SmartDialConfig.h"

USmartDialConfigFactory::USmartDialConfigFactory()
{
	SupportedClass = USmartDialConfig::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* USmartDialConfigFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	USmartDialConfig* NewSmartDialConfig = NewObject<USmartDialConfig>(InParent, Class, Name, Flags);

	return NewSmartDialConfig;
}