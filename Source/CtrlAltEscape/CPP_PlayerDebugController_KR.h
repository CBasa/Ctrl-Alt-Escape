// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_PlayerController_CB.h"
#include "CPP_PlayerDebugController_KR.generated.h"

/**
 * 
 */
UCLASS()
class CTRLALTESCAPE_API ACPP_PlayerDebugController_KR : public ACPP_PlayerController_CB
{
	GENERATED_BODY()

public:
	// Declare the custom input function
	UFUNCTION(BlueprintCallable, Category = "CustomInput")
	void SwitchControllers();

	
};
