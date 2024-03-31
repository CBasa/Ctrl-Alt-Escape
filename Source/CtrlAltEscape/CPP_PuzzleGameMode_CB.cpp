// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PuzzleGameMode_CB.h"
#include "CPP_GameInstance_CB.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_PlayerController_CB.h"
#include "CtrlAltEscapeCharacter.h"

ACPP_PuzzleGameMode_CB::ACPP_PuzzleGameMode_CB()
{
	// Enable ticking for the game mode
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	// Set tick interval to 1 second
	PrimaryActorTick.TickInterval = 1.0f; 

	//Use custom controller class
	PlayerControllerClass = ACPP_PlayerController_CB::StaticClass();
}

void ACPP_PuzzleGameMode_CB::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UCPP_GameInstance_CB* GameInstance = Cast<UCPP_GameInstance_CB>(GetGameInstance());
	if (GameInstance) {

		//GameInstance->PauseTimer();
		GameInstance->IsPlayerInPuzzle = true;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ElapsedTimeUpdate, this, &ACPP_PuzzleGameMode_CB::UpdateElapsedTime, 1.0f, true);
	
	}
}

void ACPP_PuzzleGameMode_CB::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckPuzzleTimeExceeded(); 
}

void ACPP_PuzzleGameMode_CB::CheckPuzzleTimeExceeded()
{
	if (ElapsedPuzzleTime >= PuzzleTimer) {
		IsPuzzleTimeExceeded = true;
		//Deplete stealth if puzzle time limit exceeded
		UCPP_GameInstance_CB* GameInstance = Cast<UCPP_GameInstance_CB>(GetGameInstance());
		if (GameInstance) {
			float CurrentStealth = GameInstance->GetCurrentStealth();
			float MaxStealth = GameInstance->GetMaxStealth();
			if (CurrentStealth > 0) {
				CurrentStealth -= DepleteStealthPerTick;
				GameInstance->SetCurrentStealth(CurrentStealth);
			}
		}
	}
}

void ACPP_PuzzleGameMode_CB::ReturnPlayerToLastPosition(ELevelToLoad Level)
{
	UCPP_GameInstance_CB* GameInstance = Cast<UCPP_GameInstance_CB>(GetGameInstance());
	if (GameInstance) {
		FVector SavedLocation;
		FRotator SavedRotation;
		GameInstance->GetSavedPlayerLocationAndRotation(SavedLocation, SavedRotation);

		// Create options string with player's saved location and rotation
		FString Options = FString::Printf(TEXT("playerlocation=%f,%f,%f&playerrotation=%f,%f,%f"),
			SavedLocation.X, SavedLocation.Y, SavedLocation.Z,
			SavedRotation.Pitch, SavedRotation.Yaw, SavedRotation.Roll);

		// Open level with options
		switch (Level) {
		case ELevelToLoad::OfficeLevel1:
			//UE_LOG(LogTemp, Warning, TEXT("Sending player back to Office Level 1"));
			GameInstance->IsPlayerInPuzzle = false;
			UGameplayStatics::OpenLevel(this, "L_Office", true, Options);
			break;
		case ELevelToLoad::OfficeLevel2:
			//UE_LOG(LogTemp, Warning, TEXT("Sending player back to Office Level 2"));
			GameInstance->IsPlayerInPuzzle = false;
			UGameplayStatics::OpenLevel(this, "L_Office2", true, Options);
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Invalid Level"));
			break;
		}
	}
}

void ACPP_PuzzleGameMode_CB::UpdateElapsedTime()
{
	ACPP_PlayerController_CB* const PlayerController = Cast<ACPP_PlayerController_CB>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		ACtrlAltEscapeCharacter* const PlayerCharacter = Cast<ACtrlAltEscapeCharacter>(PlayerController->GetCharacter());
		if (PlayerCharacter) {
			if (!PlayerCharacter->bIsPaused && PlayerCharacter->GetStealth() > 0) {
				ElapsedPuzzleTime += ElapsedTimeUpdateInterval;
				//UE_LOG(LogTemp, Warning, TEXT("Elapsed time in puzzle: %f"), ElapsedPuzzleTime);
			}
		}
	}

	UCPP_GameInstance_CB* GameInstance = Cast<UCPP_GameInstance_CB>(GetGameInstance());

	float CurrentStealth = GameInstance->GetCurrentStealth();
	if (CurrentStealth > 0) {
		//UE_LOG(LogTemp, Warning, TEXT("Current stealth: %f"), GameInstance->GetCurrentStealth());
	}
}
