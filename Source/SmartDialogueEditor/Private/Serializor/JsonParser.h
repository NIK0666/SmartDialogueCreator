// 

#pragma once

#include "CoreMinimal.h"
#include "SmartDialogue.h"
#include "UObject/Object.h"
#include "JsonParser.generated.h"

class USmartDialogue;

UCLASS(Blueprintable)
class SMARTDIALOGUEEDITOR_API UJsonParser : public UObject
{
	GENERATED_BODY()
	friend class USmartDialogue;
	
public:
	UFUNCTION(BlueprintCallable, Category = "JSON Parser")
	bool ParseJson(const FString& FilePath, USmartDialogue* DialogueAsset);
};