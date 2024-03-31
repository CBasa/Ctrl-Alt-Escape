// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_InteractableInterface_CB.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h" 
#include "Sound/SoundBase.h"
#include "CPP_Elevator_CB.generated.h"

UCLASS()
class CTRLALTESCAPE_API ACPP_Elevator_CB : public AActor, public ICPP_InteractableInterface_CB
{
	GENERATED_BODY()
	
public:	
	//=================================================================================================
	// Variables
	//=================================================================================================
	
	/// <summary>
	/// Location offset of the left door compared to its original location
	/// </summary>	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator")
	FVector LeftDoorOffset;

	/// <summary>
	/// Location offset of the right door compared to its original location
	/// </summary>	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator")
	FVector RightDoorOffset;

	/// <summary>
	/// Speed at which the door opens
	/// </summary>	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Elevator")
	float InterpolationSpeed;

	/// <summary>
	/// Checks if the elevator has been opened
	/// </summary>	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Elevator")
	bool IsOpen{ false };

	UPROPERTY(VisibleAnywhere, Category = "Elevator")
	UAudioComponent* AudioComponent;
	
	/// <summary>
	/// Sound to play when the door is being opened
	/// </summary>	
	UPROPERTY(EditAnywhere, Category = "Elevator")
	USoundBase* OpenSound;
	
	/// <summary>
	/// Sound to play if the player tries to open the door without access
	/// </summary>	
	UPROPERTY(EditAnywhere, Category = "Elevator")
	USoundBase* LockedSound;
	
	/// <summary>
	/// Volume of sound to play when the door is being opened
	/// </summary>	
	UPROPERTY(EditAnywhere, Category = "Elevator")
	float OpenSoundVolume{ 2.0f };
	
	/// <summary>
	/// Volume of sound to play if the player tries to open the door without access
	/// </summary>	
	UPROPERTY(EditAnywhere, Category = "Elevator")
	float LockedSoundVolume{ 2.0f };

	//=================================================================================================
	// Functions
	//=================================================================================================
	// Sets default values for this actor's properties
	ACPP_Elevator_CB();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Interaction function
	/// </summary>	
	virtual void Interact() override;

	/// <summary>
	/// Description to show player about this item
	/// </summary>	
	UFUNCTION(BlueprintCallable)
	virtual FString Description() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	/// <summary>
	/// Door opening animation
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void OpenDoors();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	//=================================================================================================
	// Variables
	//=================================================================================================

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Elevator", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Elevator", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LeftDoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Elevator", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RightDoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Elevator", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;

	// Timer handle for door update timer
	FTimerHandle DoorUpdateTimerHandle;

	FVector InitialLeftDoorLocation;

	FVector InitialRightDoorLocation;

	//=================================================================================================
	// Functions
	//=================================================================================================

	/// <summary>
	/// Update door position for opening animation
	/// </summary>	
	void UpdateDoorPositions();

};
