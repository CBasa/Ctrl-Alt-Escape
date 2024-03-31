// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PuzzleTerminal_CB.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_PuzzleGameMode_CB.h"


// Sets default values
ACPP_PuzzleTerminal_CB::ACPP_PuzzleTerminal_CB()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the root scene component
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent; // Set the root component
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox")); // Create a UBoxComponent for collision
	CollisionBox->SetupAttachment(RootSceneComponent); // Attach the collision component to the root component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootSceneComponent);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_PuzzleTerminal_CB::OnBeginOverlap);

}

// Called when the game starts or when spawned
void ACPP_PuzzleTerminal_CB::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACPP_PuzzleTerminal_CB::SavePlayerState(UCPP_GameInstance_CB* GameInstance, UWorld* World)
{
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController)
	{
		APawn* PlayerPawn = PlayerController->GetPawn();
		if (PlayerPawn)
		{
			FString LevelName = World->GetMapName();
			LevelName.RemoveFromStart(World->StreamingLevelsPrefix);
			if (LevelEnumMapping.Contains(LevelName))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Current level name: %s"), *LevelName);
				GameInstance->SetCurrentLevel(LevelEnumMapping[LevelName]);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Current level name does not match mapping definition in CPP_PuzzleTerminal.h: %s"), *LevelName);
			}
			GameInstance->SetSavedPlayerLocationAndRotation(PlayerPawn->GetActorLocation(), PlayerPawn->GetActorRotation());
		}
	}
}

void ACPP_PuzzleTerminal_CB::Interact()
{
	//UE_LOG(LogTemp, Warning, TEXT("Puzzle Terminal Interact function"));
	UWorld* World = GetWorld();
	UCPP_GameInstance_CB* GameInstance = Cast<UCPP_GameInstance_CB>(GetGameInstance());
	if (GameInstance) {
		if (GameInstance->HasAccessCard) {
			//Check if player isn't already in puzzle level before sending player to specific puzzle level - 'LevelToLoad' should be set in derived BP
			if (!GameInstance->IsPlayerInPuzzle) {
				//Save player location, rotation and current level in GameInstance
				SavePlayerState(GameInstance, World);

				switch (LevelToLoad) {
				case ELevelToLoad::PuzzleLevel1:
					//UE_LOG(LogTemp, Warning, TEXT("Sending player to Puzzle Level 1"));
					UGameplayStatics::OpenLevel(this, "L_Puzzle_1");
					break;
				case ELevelToLoad::PuzzleLevel2:
					//UE_LOG(LogTemp, Warning, TEXT("Sending player to Puzzle Level 2"));
					UGameplayStatics::OpenLevel(this, "L_Puzzle_2");
					break;
				case ELevelToLoad::PuzzleLevel3:
					//UE_LOG(LogTemp, Warning, TEXT("Sending player to Puzzle Level 3"));
					UGameplayStatics::OpenLevel(this, "L_Puzzle_3");
					break;
				default:
					UE_LOG(LogTemp, Warning, TEXT("Invalid Level"));
					break;
				}
			}
			//If player is currently in puzzle level, interacting with this item will send them back to last position
			else {
				ACPP_PuzzleGameMode_CB* PuzzleGameMode = Cast<ACPP_PuzzleGameMode_CB>(World->GetAuthGameMode());

				if (PuzzleGameMode) {
					PuzzleGameMode->ReturnPlayerToLastPosition(GameInstance->GetCurrentLevel());
				}
			}
		}
		else {
			//UE_LOG(LogTemp, Warning, TEXT("Player has not picked up access card yet, set true in BP_GameInstance_CB for debugging only"));
		}
	}
}

FString ACPP_PuzzleTerminal_CB::Description()
{
	return TEXT("A computer that can be hacked to upgrade an access card.");
}

void ACPP_PuzzleTerminal_CB::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Function to display interact/'E' icon above object
	//UE_LOG(LogTemp, Warning, TEXT("Overlapping with terminal"));
}

// Called every frame
void ACPP_PuzzleTerminal_CB::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

