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

    DialogueAsset->MakeClean();
    
    // Parse Character
    FString Character;
    if (JsonObject->TryGetStringField("Character", Character))
    {
        DialogueAsset->Character = Character;
    }

    // Parse Autobranch
    FString AutoBranch;
    if (JsonObject->TryGetStringField("Autobranch", AutoBranch))
    {
        DialogueAsset->AutoBranch = AutoBranch;
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

                TSharedPtr<FJsonObject> IfObject = PhraseObject->GetObjectField("If");
                if (IfObject.IsValid())
                {
                    Phrase.If.Key = IfObject->GetStringField("Key");
                    Phrase.If.Value = IfObject->GetIntegerField("Value");
                    FString Operation = IfObject->GetStringField("Op");
                    Phrase.If.Else = IfObject->GetBoolField("Else");

                    if (Operation == "==")
                    {
                        Phrase.If.EqualOperation = ESmartDialogueEqualOperation::EEO_Equals;
                    }
                    else if (Operation == ">=")
                    {
                        Phrase.If.EqualOperation = ESmartDialogueEqualOperation::EEO_GreaterOrEquals;
                    }
                    else if (Operation == "<=")
                    {
                        Phrase.If.EqualOperation = ESmartDialogueEqualOperation::EEO_LessOrEquals;
                    }
                    else if (Operation == ">")
                    {
                        Phrase.If.EqualOperation = ESmartDialogueEqualOperation::EEO_Greater;
                    }
                    else if (Operation == "<")
                    {
                        Phrase.If.EqualOperation = ESmartDialogueEqualOperation::EEO_Less;
                    }
                }
                
                // Parse Custom_params
                TSharedPtr<FJsonObject> CustomParamsObject = PhraseObject->GetObjectField("Custom_params");
                if (CustomParamsObject.IsValid())
                {
                    // Iterate over all keys in the CustomParamsObject
                    for (const auto& Pair : CustomParamsObject->Values)
                    {
                        // Extract key and value as FString
                        FString Key = Pair.Key;
                        FString Value = Pair.Value->AsString();

                        // Insert key-value pair into CustomParams
                        Phrase.CustomParams.Add(Key, Value);
                    }
                }

                
                Branch.Phrases.Add(Phrase);
            }

            // Parse Show
            TArray<TSharedPtr<FJsonValue>> ShowArray = BranchObject->GetArrayField("Show");
            for (TSharedPtr<FJsonValue> ShowValue : ShowArray)
            {
                Branch.Show.Add(ShowValue->AsString());
            }

            // Parse Hide
            TArray<TSharedPtr<FJsonValue>> HideArray = BranchObject->GetArrayField("Hide");
            for (TSharedPtr<FJsonValue> HideValue : HideArray)
            {
                Branch.Hide.Add(HideValue->AsString());
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
                FString Operation = VarObject->GetStringField("Op");

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

                Branch.Vars.Add(Vars);
            }

            // Ifs
            TArray<TSharedPtr<FJsonValue>> IfsArray = BranchObject->GetArrayField("If");
            for (TSharedPtr<FJsonValue> IfValue : IfsArray)
            {
                TSharedPtr<FJsonObject> IfObject = IfValue->AsObject();

                /// Parse and create FSmartDialogueVars
                FIf If;

                If.Key = IfObject->GetStringField("Key");
                If.Value = IfObject->GetIntegerField("Value");
                FString Operation = IfObject->GetStringField("Op");

                if (Operation == "==")
                {
                    If.EqualOperation = ESmartDialogueEqualOperation::EEO_Equals;
                }
                else if (Operation == ">=")
                {
                    If.EqualOperation = ESmartDialogueEqualOperation::EEO_GreaterOrEquals;
                }
                else if (Operation == "<=")
                {
                    If.EqualOperation = ESmartDialogueEqualOperation::EEO_LessOrEquals;
                }
                else if (Operation == ">")
                {
                    If.EqualOperation = ESmartDialogueEqualOperation::EEO_Greater;
                }
                else if (Operation == "<")
                {
                    If.EqualOperation = ESmartDialogueEqualOperation::EEO_Less;
                }

                Branch.If.Add(If);
            }

            DialogueAsset->Branches.Add(Branch.Name, Branch);
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
            Variable.Value = VariableObject->GetIntegerField("Value");
            
            DialogueAsset->Variables.Add(Variable);
        }
    }

    return true;
}
