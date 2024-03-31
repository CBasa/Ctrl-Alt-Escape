// Copyright Epic Games, Inc. All Rights Reserved.

#include "CtrlAltEscapeGameMode.h"
#include "CtrlAltEscapeCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_PlayerController_CB.h"


ACtrlAltEscapeGameMode::ACtrlAltEscapeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
    PlayerControllerClass = ACPP_PlayerController_CB::StaticClass();

}

void ACtrlAltEscapeGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);
}
