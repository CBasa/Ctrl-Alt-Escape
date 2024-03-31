// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_FinalExitDoor_CB.h"
#include "CPP_GameInstance_CB.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACPP_FinalExitDoor_CB::ACPP_FinalExitDoor_CB()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create the root scene component
    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    RootComponent = RootScene;

    // Create a collision box component
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent); // Attach to the root component
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_FinalExitDoor_CB::OnBeginOverlap);


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
    
    static ConstructorHelpers::FObjectFinder<USoundBase> SoundAsset3(TEXT("/Script/Engine.SoundWave'/Game/BluePrints/Assets/LockedDoors/A_DoorClose.A_DoorClose'"));
    if (SoundAsset3.Succeeded())
    {
        CloseSound = SoundAsset3.Object;
    }

}

// Called when the game starts or when spawned
void ACPP_FinalExitDoor_CB::BeginPlay()
{
	Super::BeginPlay();
	
    // Store initial door rotation
    InitialDoorRotation = DoorMesh->GetComponentRotation();
}

void ACPP_FinalExitDoor_CB::UpdateDoorPosition()
{
    // Smoothly interpolate door rotations
    FRotator CurrentDoorRotation = DoorMesh->GetComponentRotation();
    FRotator TargetDoorRotation;
    if (IsOpen) {
        TargetDoorRotation = InitialDoorRotation + DoorRotationOffset;
    }
    else {
        TargetDoorRotation = InitialDoorRotation;
    }

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
void ACPP_FinalExitDoor_CB::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_FinalExitDoor_CB::Interact()
{
    //UE_LOG(LogTemp, Warning, TEXT("Final Exit Door Interact function"));
    if (!IsOpen) {
        OpenDoors();
    }
    else {
        CloseDoors();
    }
}

FString ACPP_FinalExitDoor_CB::Description()
{
    return TEXT("There is nothing behind this door...but then why is it here?");
}

void ACPP_FinalExitDoor_CB::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //UE_LOG(LogTemp, Warning, TEXT("Overlapping with final exit door"));
}

void ACPP_FinalExitDoor_CB::OpenDoors()
{
    //Play open sound
    if (OpenSound) {
        UGameplayStatics::PlaySoundAtLocation(this, OpenSound, this->GetActorLocation(), 1.0f, 1.0f);
    }
    // Set up a timer to update door positions
    GetWorldTimerManager().SetTimer(ServerDoorUpdateTimerHandle, this, &ACPP_FinalExitDoor_CB::UpdateDoorPosition, 0.01f, true);

    IsOpen = true;
}

void ACPP_FinalExitDoor_CB::CloseDoors()
{
    //Play close sound
    if (CloseSound) {
        UGameplayStatics::PlaySoundAtLocation(this, CloseSound, this->GetActorLocation(), 0.6f, 1.0f);
    }
    // Set up a timer to update door positions
    GetWorldTimerManager().SetTimer(ServerDoorUpdateTimerHandle, this, &ACPP_FinalExitDoor_CB::UpdateDoorPosition, 0.01f, true);

    IsOpen = false;
}

