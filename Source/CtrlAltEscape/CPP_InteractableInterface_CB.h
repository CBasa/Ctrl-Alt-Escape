// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPP_InteractableInterface_CB.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCPP_InteractableInterface_CB : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CTRLALTESCAPE_API ICPP_InteractableInterface_CB
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//=================================================================================================
	// Functions
	//=================================================================================================

	/// <summary>
	/// Interact function
	/// </summary>	
	virtual void Interact() = 0;

	/// <summary>
	/// Description of the interactable item
	/// </summary>
	virtual FString Description() = 0;

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) = 0;
};
