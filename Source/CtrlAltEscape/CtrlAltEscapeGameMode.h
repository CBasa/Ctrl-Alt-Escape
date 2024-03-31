// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CtrlAltEscapeGameMode.generated.h"

UCLASS(minimalapi)
class ACtrlAltEscapeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACtrlAltEscapeGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

};



