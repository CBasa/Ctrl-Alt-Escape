// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ServerRoomDoor_CB.h"
#include "CPP_GameInstance_CB.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACPP_ServerRoomDoor_CB::ACPP_ServerRoomDoor_CB()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create the root scene component
    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    RootComponent = RootScene;

    // Create a collision box component
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent); // Attach to the root component
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_ServerRoomDoor_CB::OnBeginOverlap);


    // Create other components
    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(RootComponent); // Attach to the root component

    // Create the audio component and attach it to the root component
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->SetupAttachment(RootComponent);

    // Set the audio component to play on startup
    AudioComponent->bAutoActivate = false;

    // Set default sound
    static ConstructorHelpers::FObjectFinder<USoundBase> SoundAsset(TEXT("/Script/Engine.SoundWave'/Game/BluePrints/Assets/LockedDoors/A_ServerRoomDoor_Open.A_ServerRoomDoor_Open'"));
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
void ACPP_ServerRoomDoor_CB::BeginPlay()
{
	Super::BeginPlay();
	
    UCPP_GameInstance_CB* GameInstance = Cast<UCPP_GameInstance_CB>(GetGameInstance());

    if (GameInstance) {
        // Store initial door rotation
        InitialDoorRotation = DoorMesh->GetComponentRotation();

        if (GameInstance->IsServerRoomDoorOpen) {
            FRotator TargetDoorRotation = InitialDoorRotation + DoorRotationOffset;
            DoorMesh->SetWorldRotation(TargetDoorRotation);
        }
    }

}

void ACPP_ServerRoomDoor_CB::UpdateDoorPosition()
{
    // Smoothly interpolate door rotations
    FRotator CurrentDoorRotation = DoorMesh->GetComponentRotation();
    FRotator TargetDoorRotation = InitialDoorRotation + DoorRotationOffset;

    // Check if the door is close enough to its target rotation
    if (FMath::IsNearlyEqual(CurrentDoorRotation.Pitch, TargetDoorRotation.Pitch, 1.0f) &&
        FMath::IsNearlyEqual(CurrentDoorRotation.Yaw, TargetDoorRotation.Yaw, 1.0f) &&
        FMath::IsNearlyEqual(CurrentDoorRotation.Roll, TargetDoorRotation.Roll, 1.0f))
    {
        // Door has reached its target rotation, stop the interpolation
        GetWorldTimerManager().ClearTimer(ServerDoorUpdateTimerHandle);
    }
    else
    {
        // Door hasn't reached its target rotation yet, continue interpolation
        FRotator NewDoorRotation = FMath::RInterpTo(CurrentDoorRotation, TargetDoorRotation, GetWorld()->GetDeltaSeconds(), InterpolationSpeed);
        DoorMesh->SetWorldRotation(NewDoorRotation);
    }
}

// Called every frame
void ACPP_ServerRoomDoor_CB::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_ServerRoomDoor_CB::OpenDoors()
{
    //Play open sound
    if (OpenSound) {
        UGameplayStatics::PlaySoundAtLocation(this, OpenSound, this->GetActorLocation(), 1.0f, 1.0f);
    }
    // Set up a timer to update door positions
    GetWorldTimerManager().SetTimer(ServerDoorUpdateTimerHandle, this, &ACPP_ServerRoomDoor_CB::UpdateDoorPosition, 0.01f, true);

    IsOpen = true;
}

void ACPP_ServerRoomDoor_CB::Interact()
{
    //UE_LOG(LogTemp, Warning, TEXT("Server Room Door Interact function"));
    UCPP_GameInstance_CB* GameInstance = Cast<UCPP_GameInstance_CB>(GetGameInstance());
    if (GameInstance) {
        if (!IsOpen) {
            if (GameInstance->HasAccessCard) {
                if (GameInstance->HasServerRoomAccess) {
                    OpenDoors();
                    GameInstance->IsServerRoomDoorOpen = true;
                }
                else {
                    // PLAY SOUND EFFECT 
                    if (LockedSound) {
                        UGameplayStatics::PlaySoundAtLocation(this, LockedSound, this->GetActorLocation(), 2.0f, 1.0f);
                    }
                    //UE_LOG(LogTemp, Warning, TEXT("Player does not have access to Server Room yet, set true in BP_GameInstance_CB for debugging only"));
                }
            }
        }
    }
}

FString ACPP_ServerRoomDoor_CB::Description()
{
	return TEXT("A door leading to the server room.");
}

void ACPP_ServerRoomDoor_CB::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //Function to display interact/'E' icon above object
    //UE_LOG(LogTemp, Warning, TEXT("Overlapping with server door"));
}



