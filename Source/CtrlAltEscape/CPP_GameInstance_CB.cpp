#include "CPP_GameInstance_CB.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_GameInstance_CB.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

void UCPP_GameInstance_CB::Init()
{
	Super::Init();

	// Record the start time of the game
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ElapsedTimeUpdate, this, &UCPP_GameInstance_CB::UpdateElapsedTime, 1.0f, true);

	CurrentStealth = MaxStealth;

	//Initialise office and puzzle level arrays
	OfficeLevels.Add(ELevelToLoad::OfficeLevel1);
	OfficeLevels.Add(ELevelToLoad::OfficeLevel2);
	PuzzleLevels.Add(ELevelToLoad::PuzzleLevel1);
	PuzzleLevels.Add(ELevelToLoad::PuzzleLevel2);
	PuzzleLevels.Add(ELevelToLoad::PuzzleLevel3);
}

ELevelToLoad UCPP_GameInstance_CB::GetCurrentLevel()
{
	return CurrentLevel;
}

void UCPP_GameInstance_CB::SetCurrentLevel(ELevelToLoad Level)
{
	CurrentLevel = Level;
}

float UCPP_GameInstance_CB::GetCurrentStealth()
{
	return CurrentStealth;
}

void UCPP_GameInstance_CB::SetCurrentStealth(float Stealth)
{
	if (Stealth > MaxStealth) {
		CurrentStealth = MaxStealth;
	}
	else if (Stealth < 0) {
		CurrentStealth = 0.0f;
		//Trigger game over condition
	}
	else {
		CurrentStealth = Stealth;
	}
}

float UCPP_GameInstance_CB::GetMaxStealth()
{
	return MaxStealth;
}

float UCPP_GameInstance_CB::GetElapsedTime()
{
	return ElapsedTime;
}

void UCPP_GameInstance_CB::RestartAtCurrentLevel()
{
	//Reset stealth to half, reset IsPlayerInPuzzle if they died in puzzle level
	SetCurrentStealth(GetMaxStealth() / 2.0f);
	ResumeTimer();
	if (IsPlayerInPuzzle) {
		IsPlayerInPuzzle = false;
	}
	
	ELevelToLoad Level = GetCurrentLevel();

	switch (Level) {
	case ELevelToLoad::OfficeLevel1:
		//UE_LOG(LogTemp, Warning, TEXT("Restarting from Office Level 1"));
		UGameplayStatics::OpenLevel(this, "L_Office");
		break;
	case ELevelToLoad::OfficeLevel2:
		//UE_LOG(LogTemp, Warning, TEXT("Restarting from Office Level 2"));
		UGameplayStatics::OpenLevel(this, "L_Office2");
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Invalid Level"));
		break;
	}
}

void UCPP_GameInstance_CB::RestartGame()
{
	//Reset variables for all levels
	for (const ELevelToLoad& Level : OfficeLevels)
	{
		ResetVariables(Level);
	}

	//Reset stealth to max, rest IsPlayerInPuzzle if they died in puzzle level, reset game timer, reset current level
	SetCurrentStealth(GetMaxStealth());
	ElapsedTime = 0.0f;
	CurrentLevel = ELevelToLoad::OfficeLevel1;
	if (IsPlayerInPuzzle) {
		ResumeTimer();
		IsPlayerInPuzzle = false;
	}

	//Open first level map
	UGameplayStatics::OpenLevel(this, "L_Office");
}

void UCPP_GameInstance_CB::ResetVariables(ELevelToLoad Level)
{
	//Reset level-specific variables
	switch (Level) {
	case ELevelToLoad::OfficeLevel1:
		HasAccessCard = false;
		HasOfficeLevel2Access = false;
		HasPlayerAccessedSecondFloor = false;
		break;
	case ELevelToLoad::OfficeLevel2:
		HasServerRoomAccess = false;
		IsServerRoomDoorOpen = false;
		HasAdministratorAccess = false;
		break;
	}
}

void UCPP_GameInstance_CB::UpdateElapsedTime()
{
	ElapsedTime += ElapsedTimeUpdateInterval;
	//UE_LOG(LogTemp, Warning, TEXT("Elapsed time in game: %f"), ElapsedTime);
	
	ReplenishStealth();
	//UE_LOG(LogTemp, Warning, TEXT("Current stealth: %f"), CurrentStealth);
}

void UCPP_GameInstance_CB::PauseTimer()
{
	if (!IsTimerPaused)
	{
		GetWorld()->GetTimerManager().PauseTimer(TimerHandle_ElapsedTimeUpdate);
		IsTimerPaused = true;
	}
}

void UCPP_GameInstance_CB::ResumeTimer()
{
	if (IsTimerPaused)
	{
		GetWorld()->GetTimerManager().UnPauseTimer(TimerHandle_ElapsedTimeUpdate);
		IsTimerPaused = false;
	}
}

void UCPP_GameInstance_CB::ReplenishStealth()
{
	if (0.0f < CurrentStealth && CurrentStealth < MaxStealth) {
		float Stealth = GetCurrentStealth();
		Stealth += ReplenishStealthPerSecond;
		SetCurrentStealth(Stealth);
	}
}

void UCPP_GameInstance_CB::SetSavedPlayerLocationAndRotation(FVector Location, FRotator Rotation)
{
	SavedPlayerLocation = Location;
	SavedPlayerRotation = Rotation;
}

void UCPP_GameInstance_CB::GetSavedPlayerLocationAndRotation(FVector& OutLocation, FRotator& OutRotation)
{
	OutLocation = SavedPlayerLocation;
	OutRotation = SavedPlayerRotation;
}
