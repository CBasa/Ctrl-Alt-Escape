// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_InteractableInterface_CB.h"
#include "Components/BoxComponent.h"
#include "CPP_TheWhisperer_CB.generated.h"

UCLASS()
class CTRLALTESCAPE_API ACPP_TheWhisperer_CB : public AActor, public ICPP_InteractableInterface_CB
{
	GENERATED_BODY()
	
public:	
	//=================================================================================================
	// Functions
	//=================================================================================================
	// Sets default values for this actor's properties
	ACPP_TheWhisperer_CB();

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
	//=================================================================================================
	// Variables
	//=================================================================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "The Whisperer", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "The Whisperer", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WhispererMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "The Whisperer", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;

	//=================================================================================================
	// Functions
	//=================================================================================================
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
