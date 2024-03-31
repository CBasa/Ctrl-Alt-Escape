// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CPP_GameInstance_CB.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ELevelToLoad : uint8
{
    OfficeLevel1 UMETA(DisplayName = "Office Level 1"),
    OfficeLevel2 UMETA(DisplayName = "Office Level 2"),
    PuzzleLevel1 UMETA(DisplayName = "Puzzle Level 1"),
    PuzzleLevel2 UMETA(DisplayName = "Puzzle Level 2"),
    PuzzleLevel3 UMETA(DisplayName = "Puzzle Level 3")
    // Add more levels as needed
};



UCLASS()
class CTRLALTESCAPE_API UCPP_GameInstance_CB : public UGameInstance
{
	GENERATED_BODY()
	

public:
    //=================================================================================================
    // Variables
    //=================================================================================================
    
    /// <summary>
    /// Check if the player has picked up the access card
    /// </summary>
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
    bool HasAccessCard{ false };
    
    /// <summary>
    /// Check if the player has access to the second office level
    /// </summary>
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
    bool HasOfficeLevel2Access{ false };
    
    /// <summary>
    /// Check if the player has access to the server room
    /// </summary>
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
    bool HasServerRoomAccess{ false };    
    
    /// <summary>
    /// Check if the player has access to the final exit door
    /// </summary>
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
    bool HasAdministratorAccess{ false };
    
    /// <summary>
    /// Check if the player has already accessed the second office level before.
    /// Used to play elevator transition only on the first time the player enters the second office level.
    /// </summary>
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
    bool HasPlayerAccessedSecondFloor{ false };
    
    /// <summary>
    /// Amount of stealth to passively replenish per second
    /// </summary>
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
    float ReplenishStealthPerSecond{ 1.0f };
    
    /// <summary>
    /// Check if the player is currently in a puzzle level
    /// </summary>
    bool IsPlayerInPuzzle{ false };
    
    /// <summary>
    /// Check if the server room door has been opened
    /// </summary>
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game")
    bool IsServerRoomDoorOpen{ false };    
    
    /// <summary>
    /// The office level the player is currently on
    /// </summary>
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game")
    ELevelToLoad CurrentLevel{ ELevelToLoad::OfficeLevel1 };

    //=================================================================================================
    // Functions
    //=================================================================================================
    
    virtual void Init() override;

    /// <summary>
    /// Get the office level the player is currently on
    /// </summary>
    UFUNCTION(BlueprintCallable, Category = "Game")
    ELevelToLoad GetCurrentLevel();
    
    /// <summary>
    /// Update the player's current level
    /// </summary>   
    /// <param name="Level">The new level to update the player's current level to</param>
    UFUNCTION(BlueprintCallable, Category = "Game")
    void SetCurrentLevel(ELevelToLoad Level);
    
    /// <summary>
    /// Get the player's current stealth amount
    /// </summary>
    UFUNCTION(BlueprintCallable, Category = "Player")
    float GetCurrentStealth();
   
    /// <summary>
    /// Set the player's current stealth amount
    /// </summary>
    /// <param name="Stealth">The new stealth amount to set the player's current stealth to</param>
    UFUNCTION(BlueprintCallable, Category = "Player")
    void SetCurrentStealth(float Stealth);
    
    /// <summary>
    /// Get the player's maximum amount of stealth
    /// </summary>
    UFUNCTION(BlueprintCallable, Category = "Player")
    float GetMaxStealth();
   
    /// <summary>
    /// Save the player's current location and rotation before entering puzzle level
    /// </summary>
    /// <param name="Location">The player's current location</param>
    /// <param name="Rotation">The player's current rotation</param>
    UFUNCTION(BlueprintCallable, Category = "Player")
    void SetSavedPlayerLocationAndRotation(FVector Location, FRotator Rotation);
    
    /// <summary>
    /// Get the player's last saved location and rotation
    /// </summary>
    /// <param name="OutLocation">The player's last saved location</param>
    /// <param name="OutRotation">The player's last saved rotation</param>
    UFUNCTION(BlueprintCallable, Category = "Player")
    void GetSavedPlayerLocationAndRotation(FVector& OutLocation, FRotator& OutRotation);

    /// <summary>
    /// Get the time elapsed since the game starte
    /// </summary>
    UFUNCTION(BlueprintCallable, Category = "Game")
    float GetElapsedTime();

    /// <summary>
    /// Restart game from the player's last saved current level
    /// </summary>
    UFUNCTION(BlueprintCallable, Category = "Game")
    void RestartAtCurrentLevel();    
    
    /// <summary>
    /// Restart game from the first level
    /// </summary>
    UFUNCTION(BlueprintCallable, Category = "Game")
    void RestartGame();    
    
    /// <summary>
    /// Reset level-specific variables
    /// </summary>
    /// <param name="Level">The level to reset</param>
    UFUNCTION(BlueprintCallable, Category = "Game")
    void ResetVariables(ELevelToLoad Level);

    /// <summary>
    /// Timer handle for function to update elapsed time
    /// </summary>
    FTimerHandle TimerHandle_ElapsedTimeUpdate;

    /// <summary>
    /// Update elapsed time since game start
    /// </summary>
    void UpdateElapsedTime();

    /// <summary>
    /// Pause timer for elapsed time
    /// </summary>
    UFUNCTION(BlueprintCallable, Category = "Game")
    void PauseTimer();

    /// <summary>
    /// Resume timer for elapsed time
    /// </summary>
    UFUNCTION(BlueprintCallable, Category = "Game")
    void ResumeTimer();

    /// <summary>
    /// Replenish the player's stealth over time
    /// </summary>
    void ReplenishStealth();
    
protected:
    //=================================================================================================
    // Variables
    //=================================================================================================
    
    /// <summary>
    /// An array of all the office levels
    /// </summary>
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
    TArray<ELevelToLoad> OfficeLevels;    
    
    /// <summary>
    /// An array of all the puzzle levels
    /// </summary> 
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
    TArray<ELevelToLoad> PuzzleLevels;
    
    /// <summary>
    /// The player's current amount of stealth
    /// </summary>
    UPROPERTY(BlueprintReadWrite, Category = "Player")
    float CurrentStealth{ 100.0f };
    
    /// <summary>
    /// The player's maximum amount of stealth
    /// </summary>
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
    float MaxStealth{ 100.0f };
    
    /// <summary>
    /// Amount of time elapsed since the game began
    /// </summary>
    UPROPERTY(BlueprintReadWrite, Category = "Game")
    float ElapsedTime{ 0.0f };
    
    /// <summary>
    /// The player's saved location
    /// </summary>
    UPROPERTY(BlueprintReadWrite, Category = "Player")
    FVector SavedPlayerLocation;
    
    /// <summary>
    /// The player's saved rotation
    /// </summary>
    UPROPERTY(BlueprintReadWrite, Category = "Player")
    FRotator SavedPlayerRotation;
    
    /// <summary>
    /// Amount to increment elapsed time by per second
    /// </summary>    
    UPROPERTY(BlueprintReadOnly, Category = "Time")
    float ElapsedTimeUpdateInterval = 1.0f;
    
    /// <summary>
    /// Check if the game timer is paused
    /// </summary>
    bool IsTimerPaused = false;

};
