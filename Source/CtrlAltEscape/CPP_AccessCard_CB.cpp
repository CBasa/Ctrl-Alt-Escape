// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AccessCard_CB.h"
#include "CPP_GameInstance_CB.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACPP_AccessCard_CB::ACPP_AccessCard_CB()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create the root scene component
    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    RootComponent = RootScene;

    // Create a collision box component
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent); // Attach to the root component
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_AccessCard_CB::OnBeginOverlap);


    // Create other components
    AccessCardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AccessCardMesh"));
    AccessCardMesh->SetupAttachment(RootComponent); // Attach to the root component

    // Create the audio component and attach it to the root component
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->SetupAttachment(RootComponent);

    // Set the audio component to play on startup
    AudioComponent->bAutoActivate = false;

    // Set default sound
    static ConstructorHelpers::FObjectFinder<USoundBase> SoundAsset(TEXT("/Script/Engine.SoundWave'/Game/SFX/AccessCard/A_PickUp.A_PickUp'"));
    if (SoundAsset.Succeeded())
    {
        PickUpSound = SoundAsset.Object;
    }
}

// Called when the game starts or when spawned
void ACPP_AccessCard_CB::BeginPlay()
{
	Super::BeginPlay();
    UCPP_GameInstance_CB* GameInstance = Cast<UCPP_GameInstance_CB>(GetGameInstance());
    if (GameInstance->HasAccessCard) {
        Destroy();
    }
}

// Called every frame
void ACPP_AccessCard_CB::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_AccessCard_CB::Interact()
{
    UE_LOG(LogTemp, Warning, TEXT("Access Card Interact function"));
    UCPP_GameInstance_CB* GameInstance = Cast<UCPP_GameInstance_CB>(GetGameInstance());
    if (GameInstance) {
        GameInstance->HasAccessCard = true;
        Destroy();
        UGameplayStatics::PlaySound2D(this, PickUpSound, PickUpSoundVolume, 1.0f);

    }

}

FString ACPP_AccessCard_CB::Description()
{
    return TEXT("Someone's lost access card, can be used to access different parts of the building.");
}

void ACPP_AccessCard_CB::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //Function to display interact/'E' icon above object
    //UE_LOG(LogTemp, Warning, TEXT("Overlapping with access card"));
}

