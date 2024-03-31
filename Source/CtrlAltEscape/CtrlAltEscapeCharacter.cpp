// Copyright Epic Games, Inc. All Rights Reserved.

#include "CtrlAltEscapeCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_Log.h"
#include "CPP_PuzzleTerminal_CB.h"
#include "CtrlAltEscapeGameMode.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ACtrlAltEscapeCharacter

ACtrlAltEscapeCharacter::ACtrlAltEscapeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// Create the audio component and attach it to the root component
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	// Set the audio component to not auto play on startup
	AudioComponent->bAutoActivate = false;

	// Set default sound for opening the pause screen
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundAsset(TEXT("/Script/Engine.SoundWave'/Game/UI/GameOver/Select_TV_B.Select_TV_B'"));
	if (SoundAsset.Succeeded())
	{
		pauseSoundToPlay = SoundAsset.Object;
		//CPP_Log::Log(CLog::WARN, "In ACtrlAltEscapeCharacter: pauseSound set", 5.f);
	}

	// Set default sound for closing the pause screen
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundAsset2(TEXT("/Script/Engine.SoundWave'/Game/UI/GameOver/Interaction_Mechanic_B.Interaction_Mechanic_B'"));
	if (SoundAsset2.Succeeded())
	{
		unpauseSoundToPlay = SoundAsset2.Object;
		//CPP_Log::Log(CLog::WARN, "In ACtrlAltEscapeCharacter: unpauseSound set", 5.f);
	}
	
}

void ACtrlAltEscapeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//CPP_Log::Log(CLog::WARN, "In ACtrlAltEscapeCharacter::BeginPlay", 5.f);

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Bind the overlap event handlers
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACtrlAltEscapeCharacter::OnActorBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ACtrlAltEscapeCharacter::OnActorEndOverlap);

	// Set the player's stealth and max stealth values
	MaximumStealthValue = GetMaximumStealth();
	StealthValue = GetStealth();
}

void ACtrlAltEscapeCharacter::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UCPP_InteractableInterface_CB::StaticClass()))
	{
		OverlappingActor = OtherActor;
	}
}

void ACtrlAltEscapeCharacter::OnActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == OverlappingActor)
	{
		OverlappingActor = nullptr;
	}
}



float ACtrlAltEscapeCharacter::GetStealth()
{
	float Value;
	UCPP_GameInstance_CB* GameInstance = Cast<UCPP_GameInstance_CB>(GetGameInstance());
	//UCPP_GameInstance_CB* GameInstance = Cast<UCPP_GameInstance_CB>(GetGameInstance());
	if (GameInstance)
	{
		Value = GameInstance->GetCurrentStealth();
	}
	return Value;
}

void ACtrlAltEscapeCharacter::SetStealth(float Value)
{
	UCPP_GameInstance_CB* GameInstance = Cast<UCPP_GameInstance_CB>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->SetCurrentStealth(Value);
	}
	return;
}

float ACtrlAltEscapeCharacter::GetMaximumStealth()
{
	float Value;
	UCPP_GameInstance_CB* GameInstance = Cast<UCPP_GameInstance_CB>(GetGameInstance());
	if (GameInstance)
	{
		Value = GameInstance->GetMaxStealth();
	}
	return Value;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACtrlAltEscapeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACtrlAltEscapeCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACtrlAltEscapeCharacter::Look);

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACtrlAltEscapeCharacter::Interact);

		// Pausing
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &ACtrlAltEscapeCharacter::Pause);
			
		// Quitting
		EnhancedInputComponent->BindAction(QuitAction, ETriggerEvent::Triggered, this, &ACtrlAltEscapeCharacter::Quit);
	
	
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACtrlAltEscapeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		//CPP_Log::Log(CLog::WARN, "In Move function");

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACtrlAltEscapeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		//CPP_Log::Log(CLog::WARN, "In Look function");

		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACtrlAltEscapeCharacter::Interact()
{
	//CPP_Log::Log(CLog::WARN, "In Interact function - outer");
	if (Controller != nullptr)
	{
		//Check if object is interactable
		//CPP_Log::Log(CLog::WARN, "In Interact function", 2.0f);
		
		if (CanInteract())
		{
			// Call the interact function on the interactable actor
			 if (ICPP_InteractableInterface_CB* InteractableActor = Cast<ICPP_InteractableInterface_CB>(OverlappingActor))
			 {
				 InteractableActor->Interact();
			 }

		}

	}
}

void ACtrlAltEscapeCharacter::Quit()
{
	//UE_LOG(LogTemp, Warning, TEXT("Quitting game"));
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Type::Quit, false);
}

bool ACtrlAltEscapeCharacter::CanInteract() const
{
	UCPP_GameInstance_CB* GameInstance = Cast<UCPP_GameInstance_CB>(GetGameInstance());
	if (GameInstance) {
		if (OverlappingActor)
		{
			//Disable puzzle terminals once puzzle has been completed
			ACPP_PuzzleTerminal_CB* PuzzleTerminal = Cast<ACPP_PuzzleTerminal_CB>(OverlappingActor);

			if (PuzzleTerminal) {
				ACtrlAltEscapeGameMode* CurrentGameMode = Cast<ACtrlAltEscapeGameMode>(GetWorld()->GetAuthGameMode());
			
				if (CurrentGameMode) {
					//Differentiate by tag
					int Key = *PuzzleTerminalMapping.FindKey(PuzzleTerminal->Tags[0]);
					//UE_LOG(LogTemp, Warning, TEXT("Terminal tag: %d"), Key);
					switch (Key) {
					case 1:
						if (GameInstance->HasOfficeLevel2Access) {
							//UE_LOG(LogTemp, Warning, TEXT("Puzzle 1 already completed, terminal is disabled"));
							return false;
						}
						break;
					case 2:
						if (GameInstance->HasServerRoomAccess) {
							//UE_LOG(LogTemp, Warning, TEXT("Puzzle 2 already completed, terminal is disabled"));
							return false;
						}
						break;
					case 3:
						if (GameInstance->HasAdministratorAccess) {
							//UE_LOG(LogTemp, Warning, TEXT("Puzzle 3 already completed, terminal is disabled"));
							return false;
						}
						break;
					}
				}

			}
			// Check if the overlapping actor implements the interactable interface
			return OverlappingActor->GetClass()->ImplementsInterface(UCPP_InteractableInterface_CB::StaticClass());
		}
	}
	return false;
}


void ACtrlAltEscapeCharacter::Pause()
{
	UCPP_GameInstance_CB* GameInstance = Cast<UCPP_GameInstance_CB>(GetGameInstance());
	if (GameInstance) {

		if (Controller != nullptr)
		{
			//CPP_Log::Log(CLog::INFO, "In Pause Method", 5.f);
			if (bIsPaused)
			{
				// Unpause game
				bIsPaused = false;
				ToggleGamePause(bIsPaused);
				// Remove pause screen widget
				if (PausedWidget)
				{
					PausedWidget->RemoveFromParent();
					//Cant destroy widget directly, let the GC take care of it
					PausedWidget = nullptr;
				}

				// Unpause timer
				GameInstance->UCPP_GameInstance_CB::ResumeTimer();

				//Play pause screen off sound
				if (unpauseSoundToPlay) {
					UGameplayStatics::PlaySound2D(this, unpauseSoundToPlay, 0.5f, 1.0f);
				}

			}
			else
			{
				// Pause game
				//Play pause screen sound
				if (pauseSoundToPlay) {
					UGameplayStatics::PlaySound2D(this, pauseSoundToPlay, 0.5f, 1.0f);
				}

				// Pause timer
				GameInstance->UCPP_GameInstance_CB::PauseTimer();

				bIsPaused = true;
				ToggleGamePause(bIsPaused);

				// Display Paused game notice
				UUserWidget* WidgetPause = CreateWidget<UUserWidget>(GetGameInstance(), WidgetClass);
				WidgetPause->AddToViewport();
				PausedWidget = WidgetPause; // Save created widget in global variable for reference later when removing widget
				//CPP_Log::Log(CLog::INFO, "WidgetMade", 5.f);





			}

		}
	}
}

void ACtrlAltEscapeCharacter::ToggleGamePause(bool bValue)
{
	//CPP_Log::Log(CLog::INFO, "In ToggleGamePause Method", 5.f);
	//UE_LOG(LogTemp, Warning, TEXT("bIsPaused value is: %d"), bValue);
	//CPP_Log::Log(CLog::INFO, "bIsPaused value is: ", bValue, 5.f);
	if(ACPP_PlayerController_CB* const ThePlayer = Cast<ACPP_PlayerController_CB>(GetWorld()->GetFirstPlayerController()))
	//if (ThePlayer != NULL)
	{
		//CPP_Log::Log(CLog::INFO, "bIsPaused value (after cast) is: ", bValue, 5.f);
	// Set the game paused state
		ThePlayer->SetPause(bValue);
		//CPP_Log::Log(CLog::INFO, "bIsPaused value (after setting pause/unpause) is: ", bValue, 5.f);
		

	// Optionally, show/hide the mouse cursor
		ThePlayer->SetShowMouseCursor(bValue);

	// Optionally, enable/disable click events
		//ThePlayer->SetEnableClickEvents(bIsPaused);

	}

	return;
}



