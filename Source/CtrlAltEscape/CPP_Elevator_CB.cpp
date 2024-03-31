// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Elevator_CB.h"
#include "CPP_GameInstance_CB.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACPP_Elevator_CB::ACPP_Elevator_CB()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create the root scene component
    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    RootComponent = RootScene;

    // Create a collision box component
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent); // Attach to the root component
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Elevator_CB::OnBeginOverlap);


    // Create other components
    LeftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoorMesh"));
    LeftDoorMesh->SetupAttachment(RootComponent); // Attach to the root component

    RightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoorMesh"));
    RightDoorMesh->SetupAttachment(RootComponent); // Attach to the root component


    // Create the audio component and attach it to the root component
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->SetupAttachment(RootComponent);

    // Set the audio component to play on startup
    AudioComponent->bAutoActivate = false;

    // Set default sound
    static ConstructorHelpers::FObjectFinder<USoundBase> SoundAsset(TEXT("/Script/Engine.SoundWave'/Game/BluePrints/Assets/LockedDoors/A_ElevatorDing.A_ElevatorDing'"));
    if (SoundAsset.Succeeded())
    {
        OpenSound = SoundAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> SoundAsset2(TEXT("/Script/Engine.SoundWave'/Game/BluePrints/Assets/LockedDoors/A_ErrorBuzz.A_ErrorBuzz'"));
    if (SoundAsset2.Succeeded())
    {
        LockedSound = SoundAsset2.Object;
    }
}

// Called when the game starts or when spawned
void ACPP_Elevator_CB::BeginPlay()
{
	Super::BeginPlay();
	
    // Store initial door locations
    InitialLeftDoorLocation = LeftDoorMesh->GetComponentLocation();
    InitialRightDoorLocation = RightDoorMesh->GetComponentLocation();
}

void ACPP_Elevator_CB::UpdateDoorPositions()
{

    // Smoothly interpolate door positions
    FVector CurrentLeftDoorLocation = LeftDoorMesh->GetComponentLocation();
    FVector CurrentRightDoorLocation = RightDoorMesh->GetComponentLocation();

    FVector TargetLeftDoorLocation = InitialLeftDoorLocation + LeftDoorOffset;
    FVector TargetRightDoorLocation = InitialLeftDoorLocation + RightDoorOffset;

    // Check if the doors are close enough to their target positions
    if ((CurrentLeftDoorLocation - TargetLeftDoorLocation).SizeSquared() < 1.0f &&
        (CurrentRightDoorLocation - TargetRightDoorLocation).SizeSquared() < 1.0f)
    {
        // Doors have reached their target positions, stop the timer
        GetWorldTimerManager().ClearTimer(DoorUpdateTimerHandle);
    }
    else
    {
        // Doors haven't reached their target positions yet, continue interpolation
        LeftDoorMesh->SetWorldLocation(FMath::VInterpTo(CurrentLeftDoorLocation, TargetLeftDoorLocation, GetWorld()->GetDeltaSeconds(), InterpolationSpeed));
        RightDoorMesh->SetWorldLocation(FMath::VInterpTo(CurrentRightDoorLocation, TargetRightDoorLocation, GetWorld()->GetDeltaSeconds(), InterpolationSpeed));
    }
}

// Called every frame
void ACPP_Elevator_CB::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Elevator_CB::OpenDoors()
{
    //Play open sound
    if (OpenSound) {
        UGameplayStatics::PlaySoundAtLocation(this, OpenSound, this->GetActorLocation(), OpenSoundVolume, 1.0f);
    }
    
    // Set up a timer to update door positions
    GetWorldTimerManager().SetTimer(DoorUpdateTimerHandle, this, &ACPP_Elevator_CB::UpdateDoorPositions, 0.01f, true);

    IsOpen = true;
}

void ACPP_Elevator_CB::Interact()
{
    //UE_LOG(LogTemp, Warning, TEXT("Elevator Interact function"));
    UCPP_GameInstance_CB* GameInstance = Cast<UCPP_GameInstance_CB>(GetGameInstance());
    if (GameInstance) {
        if (!IsOpen) {
            if (GameInstance->HasAccessCard) {
                if (GameInstance->HasOfficeLevel2Access) {
                    OpenDoors();
                }
                else {
                    // PLAY SOUND EFFECT 
                    if (LockedSound) {
                        UGameplayStatics::PlaySoundAtLocation(this, LockedSound, this->GetActorLocation(), LockedSoundVolume, 1.0f);
                    }
                    //UE_LOG(LogTemp, Warning, TEXT("Player does not have access to Office Level 2 yet, set true in BP_GameInstance_CB for debugging only"));
                }
            }
        }
    }

}

FString ACPP_Elevator_CB::Description()
{
	return TEXT("An elevator that can be used to get to different floors, requires a company access card to use.");
}

void ACPP_Elevator_CB::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //Function to display interact/'E' icon above object
    //UE_LOG(LogTemp, Warning, TEXT("Overlapping with elevator"));
}



