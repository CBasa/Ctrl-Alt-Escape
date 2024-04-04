// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CPP_GameInstance_CB.h"
#include "CPP_PuzzleGameMode_CB.generated.h"

/**
 * 
 */
UCLASS()
class CTRLALTESCAPE_API ACPP_PuzzleGameMode_CB : public AGameModeBase
{
	GENERATED_BODY()

public:
	//=================================================================================================
	// Variables
	//=================================================================================================

	/// <summary>
	/// Map of level enum to the name of the level map
	/// </summary>	
	TMap<FString, ELevelToLoad> LevelEnumMapping = {
		{TEXT("L_Puzzle_1"), ELevelToLoad::PuzzleLevel1},
		{TEXT("L_Puzzle_2"), ELevelToLoad::PuzzleLevel2},
		{TEXT("L_Puzzle_3"), ELevelToLoad::PuzzleLevel3}
		// Add more mappings as needed
	};

	/// <summary>
	/// Amount of time elapsed in puzzle
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Puzzle")
	float ElapsedPuzzleTime;
	
	/// <summary>
	/// Maximum amount of time allowed for puzzle level 1 in seconds
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Puzzle")
	float PuzzleTimer_Level1{ 180.0f };	
	
	/// <summary>
	/// Maximum amount of time allowed for puzzle level 2 in seconds
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Puzzle")
	float PuzzleTimer_Level2{ 300.0f };
	
	/// <summary>
	/// Maximum amount of time allowed for puzzle level 3 in seconds
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Puzzle")
	float PuzzleTimer_Level3{ 420.0f };
	
	/// <summary>
	/// Variable to assign level-specific timer value to
	/// </summary>
	UPROPERTY(BlueprintReadWrite, Category = "Puzzle")
	float PuzzleTimer{ 300.0f };
	
	/// <summary>
	/// Amount of stealth to deplete per tick
	/// </summary>
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Puzzle")
	float DepleteStealthPerTick{ 1.0f };
	
	/// <summary>
	/// Check if the elapsed puzzle time has exceeded the maximum amount of time allowed
	/// </summary>	
	UPROPERTY(BlueprintReadWrite, Category = "Puzzle")
	bool IsPuzzleTimeExceeded{ false };
	
	/// <summary>
	/// Amount to increment the elapsed puzzle time by per second
	/// </summary>
	UPROPERTY(BlueprintReadOnly, Category = "Puzzle")
	float ElapsedTimeUpdateInterval = 1.0f;

	//=================================================================================================
	// Functions
	//=================================================================================================
	ACPP_PuzzleGameMode_CB();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void Tick(float DeltaSeconds) override;

	/// <summary>
	/// Deplete the player's stealth over time if the puzzle time limit has been exceeded
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Puzzle")
	void CheckPuzzleTimeExceeded();

	/// <summary>
	/// Return the player to their last position in the office
	/// </summary>
	/// <param name="Level">The player's current office level</param>
	UFUNCTION(BlueprintCallable, Category = "Puzzle")
	void ReturnPlayerToLastPosition(ELevelToLoad Level);

	/// <summary>
	/// Timer handle for function to update elapsed time in puzzle
	/// </summary>
	FTimerHandle TimerHandle_ElapsedTimeUpdate;

	/// <summary>
	/// Update elapsed time in puzzle
	/// </summary>
	void UpdateElapsedTime();
};
