// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerController_CB.h"
#include "CtrlAltEscapeGameMode.h"


void ACPP_PlayerController_CB::BeginPlay()
{
    Super::BeginPlay();

    ParseOptions();
}

void ACPP_PlayerController_CB::ParseOptions()
{
    ACtrlAltEscapeGameMode* GameMode = Cast<ACtrlAltEscapeGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        FString Options = GameMode->OptionsString;

        //UE_LOG(LogTemp, Warning, TEXT("Options passed to OpenLevel: %s"), *Options);

        if (!Options.IsEmpty())
        {
            APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
            if (PlayerController)
            {
                APawn* PlayerPawn = PlayerController->GetPawn();
                if (PlayerPawn)
                {
                    // Split by '&' to separate key-value pairs
                    TArray<FString> KeyValuePairs;
                    Options.ParseIntoArray(KeyValuePairs, TEXT("&"), true);

                    FVector NewLocation;
                    FRotator NewRotation;

                    for (const FString& KeyValuePair : KeyValuePairs)
                    {
                        FString Key;
                        FString Value;

                        // Split each key-value pair by '=' to separate key and value
                        if (KeyValuePair.Split("=", &Key, &Value))
                        {
                            // Now you have Key and Value for each pair
                            // Split the value by ',' to get individual numbers
                            TArray<FString> Numbers;
                            Value.ParseIntoArray(Numbers, TEXT(","), true);

                            if (Numbers.Num() == 3)
                            {
                                // Process the numbers based on the key
                                if (Key == "?playerlocation")
                                {
                                    NewLocation.X = FCString::Atof(*Numbers[0]);
                                    NewLocation.Y = FCString::Atof(*Numbers[1]);
                                    NewLocation.Z = FCString::Atof(*Numbers[2]);

                                }
                                else if (Key == "playerrotation")
                                {
                                    NewRotation.Pitch = FCString::Atof(*Numbers[0]);
                                    NewRotation.Yaw = FCString::Atof(*Numbers[1]);
                                    NewRotation.Roll = FCString::Atof(*Numbers[2]);

                                }
                            }
                        }
                    }

                    // Set player's location and rotation
                    PlayerPawn->SetActorLocationAndRotation(NewLocation, NewRotation);
                }
            }
        }
    }
}
