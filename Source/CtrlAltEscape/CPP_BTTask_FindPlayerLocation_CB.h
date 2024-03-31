// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CPP_BTTask_FindPlayerLocation_CB.generated.h"

/**
 * 
 */
UCLASS()
class CTRLALTESCAPE_API UCPP_BTTask_FindPlayerLocation_CB : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UCPP_BTTask_FindPlayerLocation_CB();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
