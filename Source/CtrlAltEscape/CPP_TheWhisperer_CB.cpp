// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_TheWhisperer_CB.h"

// Sets default values
ACPP_TheWhisperer_CB::ACPP_TheWhisperer_CB()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create the root scene component
    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    RootComponent = RootScene;

    // Create a collision box component
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent); // Attach to the root component
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_TheWhisperer_CB::OnBeginOverlap);


    // Create other components
    WhispererMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WhispererMesh"));
    WhispererMesh->SetupAttachment(RootComponent); // Attach to the root component

}

// Called when the game starts or when spawned
void ACPP_TheWhisperer_CB::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_TheWhisperer_CB::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_TheWhisperer_CB::Interact()
{
	//UE_LOG(LogTemp, Warning, TEXT("The Whisperer Interact function"));
}

FString ACPP_TheWhisperer_CB::Description()
{
	return TEXT("A mysterious bust of a...crocodile?");
}

void ACPP_TheWhisperer_CB::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //UE_LOG(LogTemp, Warning, TEXT("Overlapping with The Whisperer"));
}

