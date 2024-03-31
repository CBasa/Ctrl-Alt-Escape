// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_InteractableInterface_CB.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h" 
#include "Sound/SoundBase.h"
#include "CPP_FinalExitDoor_CB.generated.h"

UCLASS()
class CTRLALTESCAPE_API ACPP_FinalExitDoor_CB : public AActor, public ICPP_InteractableInterface_CB
{
	GENERATED_BODY()
	
public:

	//=================================================================================================
	// Variables
	//=================================================================================================

	/// <summary>
	/// Angle of door when open compared to its initial rotation
	/// </summary>	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Final Exit Door")
	FRotator DoorRotationOffset;

	/// <summary>
	/// Speed at which the door opens
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Final Exit Door")
	float InterpolationSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Final Exit Door")
	UAudioComponent* AudioComponent;
	
	/// <summary>
	/// Sound to play when the door is being opened
	/// </summary>	
	UPROPERTY(EditAnywhere, Category = "Final Exit Door")
	USoundBase* OpenSound;	

	/// <summary>
	/// Sound to play when the door is being closed
	/// </summary>	
	UPROPERTY(EditAnywhere, Category = "Final Exit Door")
	USoundBase* CloseSound;

	/// <summary>
	/// Sound to play if the player tries to open the door without access
	/// </summary>	
	UPROPERTY(EditAnywhere, Category = "Final Exit Door")
	USoundBase* LockedSound;	

	/// <summary>
	/// Checks if the door has been opened
	/// </summary>	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Final Exit Door")
	bool IsOpen{ false };

	//=================================================================================================
	// Functions
	//=================================================================================================
	// Sets default values for this actor's properties
	ACPP_FinalExitDoor_CB();

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

	/// <summary>
	/// Door closing animation
	/// </summary>	
	UFUNCTION(BlueprintCallable)
	void CloseDoors();

protected:
	//=================================================================================================
	// Functions
	//=================================================================================================
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//=================================================================================================
	// Variables
	//=================================================================================================
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Access Card", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Access Card", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Access Card", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;

	// Timer handle for door update timer
	FTimerHandle ServerDoorUpdateTimerHandle;

	FRotator InitialDoorRotation;

	//=================================================================================================
	// Functions
	//=================================================================================================
	
	/// <summary>
	/// Update door position for opening animation
	/// </summary>	
	void UpdateDoorPosition();



};
