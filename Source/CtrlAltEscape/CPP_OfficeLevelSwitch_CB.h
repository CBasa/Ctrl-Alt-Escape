// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CPP_OfficeLevelSwitch_CB.generated.h"

UCLASS()
class CTRLALTESCAPE_API ACPP_OfficeLevelSwitch_CB : public AActor
{
	GENERATED_BODY()
	
public:	
	//=================================================================================================
	// Variables
	//=================================================================================================

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionBox;


	//=================================================================================================
	// Functions
	//=================================================================================================

	// Sets default values for this actor's properties
	ACPP_OfficeLevelSwitch_CB();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>
	/// Send the player to the second office level on overlap
	/// </summary>
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
