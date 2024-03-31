// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_InteractableInterface_CB.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h" 
#include "Sound/SoundBase.h"
#include "CPP_AccessCard_CB.generated.h"

UCLASS()
class CTRLALTESCAPE_API ACPP_AccessCard_CB : public AActor, public ICPP_InteractableInterface_CB
{
	GENERATED_BODY()
	
public:	
	//=================================================================================================
	// Variables
	//=================================================================================================

	UPROPERTY(VisibleAnywhere, Category = "Access Card")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, Category = "Access Card")
	USoundBase* PickUpSound;

	/// <summary>
	/// Sound to play when the access card is picked up
	/// </summary>	
	UPROPERTY(EditAnywhere, Category = "Access Card")
	float PickUpSoundVolume{1.0f};


	//=================================================================================================
	// Functions
	//=================================================================================================
	// Sets default values for this actor's properties
	ACPP_AccessCard_CB();

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//=================================================================================================
	// Variables
	//=================================================================================================

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Access Card", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Access Card", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* AccessCardMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Access Card", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;

};
