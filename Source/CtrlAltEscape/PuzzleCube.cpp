// "Ctrl Alt Escape" uses Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere.”“Unreal® Engine, Copyright 1998 – 2024, Epic Games, Inc. All rights reserved.”


#include "PuzzleCube.h"

// Sets default values
APuzzleCube::APuzzleCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APuzzleCube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APuzzleCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

