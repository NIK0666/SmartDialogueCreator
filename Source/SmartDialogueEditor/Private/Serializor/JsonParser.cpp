// 


#include "JsonParser.h"

#include "SmartDialogue.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"


bool UJsonParser::ParseJson(const FString& FilePath, USmartDialogue* DialogueAsset)
{
    FString JsonString;

    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load JSON file: %s"), *FilePath);
        return false;
    }

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

    if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to deserialize JSON file: %s"), *FilePath);
        return false;
    }
	
    // Parse Character
    FString Character;
    if (JsonObject->TryGetStringField("Character", Character))
    {
        DialogueAsset->SetCharacter(Character);
    }

    // Parse Autobranch
    FString AutoBranch;
    if (JsonObject->TryGetStringField("Autobranch", AutoBranch))
    {
        DialogueAsset->SetAutoBranch(AutoBranch);
    }
    const TArray<TSharedPtr<FJsonValue>>* BranchesArray;
    if (JsonObject->TryGetArrayField("Branches", BranchesArray))
    {
        for (const TSharedPtr<FJsonValue>& BranchValue : *BranchesArray)
        {
            TSharedPtr<FJsonObject> BranchObject = BranchValue->AsObject();

            // Parse and create FSmartDialogueBranch
            FSmartDialogueBranch Branch;

            Branch.Name = *BranchObject->GetStringField("Name");
            Branch.Text = FText::FromString(BranchObject->GetStringField("Text"));
            Branch.Choice = BranchObject->GetBoolField("Choice");
            Branch.Closed = BranchObject->GetBoolField("Closed");
            Branch.OrCond = BranchObject->GetBoolField("Or_cond");
            Branch.If = BranchObject->GetStringField("If");
            Branch.Hidden = BranchObject->GetBoolField("Hidden");
            Branch.HideSelf = BranchObject->GetBoolField("Hide_self");
            Branch.ChangeStarted = BranchObject->GetStringField("Change_started");

            // Parse Phrases
            TArray<TSharedPtr<FJsonValue>> PhrasesArray = BranchObject->GetArrayField("Phrases");
            for (TSharedPtr<FJsonValue> PhraseValue : PhrasesArray)
            {
                TSharedPtr<FJsonObject> PhraseObject = PhraseValue->AsObject();

                // Parse and create FSmartDialoguePhrase
                FSmartDialoguePhrase Phrase;

                Phrase.Anim = PhraseObject->GetStringField("Anim");
                Phrase.NPC = PhraseObject->GetStringField("Npc");
                Phrase.Random = PhraseObject->GetBoolField("Random");
                Phrase.Text = FText::FromString(PhraseObject->GetStringField("Text"));

                // Parse Custom_params
                // TSharedPtr<FJsonObject> CustomParamsObject = PhraseObject->GetObjectField("Custom_params");
                // if (CustomParamsObject.IsValid())
                // {
                //     FString ScreenImage, Sound;
                //     if (CustomParamsObject->TryGetStringField("ScreenImage", ScreenImage))
                //     {
                //         Phrase.CustomParams.Add("ScreenImage", ScreenImage);
                //     }
                //     if (CustomParamsObject->TryGetStringField("Sound", Sound))
                //     {
                //         Phrase.CustomParams.Add("Sound", Sound);
                //     }
                // }

                // Parse Loc
                // TSharedPtr<FJsonObject> LocObject = PhraseObject->GetObjectField("Loc");
                // if (LocObject.IsValid())
                // {
                //     for (const auto& Pair : LocObject->Values)
                //     {
                //         Phrase.Text = FText::FromString(Pair.Value->AsString());
                //     }
                // }

                Branch.Phrases.Add(Phrase);
            }

            // Parse Show
            TArray<TSharedPtr<FJsonValue>> ShowArray = BranchObject->GetArrayField("Show");
            for (TSharedPtr<FJsonValue> ShowValue : ShowArray)
            {
                Branch.Show.Add(ShowValue->AsString());
            }

            // Parse Event
            TSharedPtr<FJsonObject> EventObject = BranchObject->GetObjectField("Event");
            if (EventObject.IsValid())
            {
                Branch.Event.Name = EventObject->GetStringField("Name");
                Branch.Event.Param = EventObject->GetStringField("Param");
                Branch.Event.Post = EventObject->GetBoolField("Post");
            }

            // Parse Vars
            TArray<TSharedPtr<FJsonValue>> VarsArray = BranchObject->GetArrayField("Vars");
            for (TSharedPtr<FJsonValue> VarValue : VarsArray)
            {
                TSharedPtr<FJsonObject> VarObject = VarValue->AsObject();

                /// Parse and create FSmartDialogueVars
                FSmartDialogueVars Vars;

                Vars.Key = VarObject->GetStringField("Key");
                Vars.Value = VarObject->GetIntegerField("Value");
                FString Operation = VarObject->GetStringField("Operation");

                if (Operation == "=")
                {
                    Vars.Operation = ESmartDialogueOperation::EO_Equals;
                }
                else if (Operation == "+")
                {
                    Vars.Operation = ESmartDialogueOperation::EO_Plus;
                }
                else if (Operation == "-")
                {
                    Vars.Operation = ESmartDialogueOperation::EO_Minus;
                }
                else if (Operation == "*")
                {
                    Vars.Operation = ESmartDialogueOperation::EO_Multiply;
                }
                else if (Operation == "/")
                {
                    Vars.Operation = ESmartDialogueOperation::EO_Divide;
                }

                Branch.Vars = Vars;
            }

            DialogueAsset->AddNewBranch(Branch);
        }
    }

    const TArray< TSharedPtr<FJsonValue> >* VariablesArray;
    if(JsonObject->TryGetArrayField(TEXT("Variables"), VariablesArray))
    {
        for (const TSharedPtr<FJsonValue>& VariableValue : *VariablesArray)
        {
            TSharedPtr<FJsonObject> VariableObject = VariableValue->AsObject();

            // Parse and create FVariableData
            FVariableData Variable;

            Variable.Desc = VariableObject->GetStringField("Desc");
            Variable.Key = VariableObject->GetStringField("Key");
            Variable.Value = VariableObject->GetStringField("Value");

            DialogueAsset->AddNewVariable(Variable);
        }
    }

    return true;
}
