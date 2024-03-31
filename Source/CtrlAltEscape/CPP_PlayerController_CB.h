// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPP_PlayerController_CB.generated.h"

/**
 *
 */
UCLASS()
class CTRLALTESCAPE_API ACPP_PlayerController_CB : public APlayerController
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	Function to spawn player at a given location and rotation through options passed in URL.
	Example url: "playerlocation=100,200,300&amp;playerrotation=0,45,0". 

	Options format:
	FString Options = FString::Printf(TEXT("playerlocation=%f,%f,%f&amp;playerrotation=%f,%f,%f"),
		SavedLocation.X, SavedLocation.Y, SavedLocation.Z,
		SavedRotation.Pitch, SavedRotation.Yaw, SavedRotation.Roll) 
	*/
	void ParseOptions();
};
