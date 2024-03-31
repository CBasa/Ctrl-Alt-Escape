// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_OfficeLevelSwitch_CB.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_GameInstance_CB.h"


// Sets default values
ACPP_OfficeLevelSwitch_CB::ACPP_OfficeLevelSwitch_CB()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Create the root scene component
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent; // Set the root component
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox")); // Create a UBoxComponent for collision
	CollisionBox->SetupAttachment(RootSceneComponent); // Attach the collision component to the root component

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_OfficeLevelSwitch_CB::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ACPP_OfficeLevelSwitch_CB::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACPP_OfficeLevelSwitch_CB::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UCPP_GameInstance_CB* GameInstance = Cast<UCPP_GameInstance_CB>(GetGameInstance());

	if (GameInstance) {
		switch (GameInstance->GetCurrentLevel()) {
		case ELevelToLoad::OfficeLevel1:
			//UE_LOG(LogTemp, Warning, TEXT("Sending player to Office Level 2"));
			UGameplayStatics::OpenLevel(this, "L_Office2");
			GameInstance->SetCurrentLevel(ELevelToLoad::OfficeLevel2);
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Office level switch failed: Current level has no level to proceed to"));
			break;
		}
	}
}

// Called every frame
void ACPP_OfficeLevelSwitch_CB::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

