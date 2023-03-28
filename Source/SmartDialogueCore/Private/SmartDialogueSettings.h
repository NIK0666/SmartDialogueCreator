// 

#pragma once

#include "CoreMinimal.h"
#include "SmartDialogueSettings.generated.h"

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Smart Dialogue"))
class SMARTDIALOGUECORE_API USmartDialogueSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, config, Category = "Smart Dialogue", meta = (DisplayName = "Dialogues Config Asset"))
	FSoftObjectPath DialoguesConfigAsset;
};