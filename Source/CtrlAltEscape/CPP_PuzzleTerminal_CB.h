// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_InteractableInterface_CB.h"
#include "CPP_GameInstance_CB.h"
#include "Components/BoxComponent.h"
#include "CPP_PuzzleTerminal_CB.generated.h"



UCLASS()
class CTRLALTESCAPE_API ACPP_PuzzleTerminal_CB : public AActor, public ICPP_InteractableInterface_CB
{
	GENERATED_BODY()
	
public:	
	//=================================================================================================
	// Variables
	//=================================================================================================
	
	/// <summary>
	/// Map of level enum to the name of the level map
	/// </summary>	
	TMap<FString, ELevelToLoad> LevelEnumMapping = {
		{TEXT("L_Office"), ELevelToLoad::OfficeLevel1},
		{TEXT("L_Office2"), ELevelToLoad::OfficeLevel2}
		// Add more mappings as needed
	};

	/// <summary>
	/// The puzzle level for the terminal to send the player to
	/// </summary>	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Puzzle Level")
	ELevelToLoad LevelToLoad;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* StaticMeshComponent;

	//=================================================================================================
	// Functions
	//=================================================================================================

	// Sets default values for this actor's properties
	ACPP_PuzzleTerminal_CB();

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
	
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionBox;


	//=================================================================================================
	// Functions
	//=================================================================================================

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>
	/// Save the player's current location and rotation before loading puzzle level
	/// </summary>	
	/// <param name="GameInstance">The active game instance</param>
	/// <param name="World">The active world</param>
	void SavePlayerState(UCPP_GameInstance_CB* GameInstance, UWorld* World);




};
