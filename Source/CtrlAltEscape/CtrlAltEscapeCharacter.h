// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPP_PlayerController_CB.h"
#include "CPP_InteractableInterface_CB.h"
#include "CPP_GameInstance_CB.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Logging/LogMacros.h"
#include "Components/AudioComponent.h" 
#include "Sound/SoundBase.h"
#include "CtrlAltEscapeCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ACtrlAltEscapeCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Pause Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;	
	
	/** Quit Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* QuitAction;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> WidgetClass;

	UPROPERTY()
	class UUserWidget* PausedWidget;

	UPROPERTY(VisibleAnywhere, Category = "Pause screen")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, Category = "Pause screen")
	USoundBase* pauseSoundToPlay;

	UPROPERTY(EditAnywhere, Category = "Pause screen")
	USoundBase* unpauseSoundToPlay;


public:
	ACtrlAltEscapeCharacter();
	
	// Game pause toggle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
	bool bIsPaused = false;

	// Called to pause the game
	UFUNCTION(BlueprintCallable, Category = "Player")
	void Pause();
	
	// Implements pause function
	void ToggleGamePause(bool bValue);

	// Player's stealth value (Editable for debugging as needed)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
	float StealthValue;

	// Player's maximum stealth value (Editable for debugging as needed)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
	float MaximumStealthValue;


	/** Called for interacting input */
	void Interact();	
	
	/** Called for quitting input */
	void Quit();

	// Check if the player can interact
	bool CanInteract() const;

	// Get current stealth value (stored in Game Instance)
	UFUNCTION(BlueprintCallable, Category = "Game")
	float GetStealth();

	// Set current stealth value (store value in Game Instance)
	UFUNCTION(BlueprintCallable, Category = "Game")
	void SetStealth(float Value);

	// Get max stealth value stored in Game Instance
	UFUNCTION(BlueprintCallable, Category = "Game")
	float GetMaximumStealth();


protected:

	TMap<int, FName> PuzzleTerminalMapping = {
		{1, FName(TEXT("PuzzleTerminal1"))},
		{2, FName(TEXT("PuzzleTerminal2"))},
		{3, FName(TEXT("PuzzleTerminal3"))}
	};

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);


			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();


	// The actor currently being overlapped by the player
	AActor* OverlappingActor; // Declare the OverlappingActor variable

	// Event handler for  when player begins overlapping with another actor
	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Event handler for when the player ends overlapping with another actor
	UFUNCTION()
	void OnActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

