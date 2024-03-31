// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BTTask_FindPlayerLocation_CB.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UCPP_BTTask_FindPlayerLocation_CB::UCPP_BTTask_FindPlayerLocation_CB()
{
	NodeName = TEXT("Find Player Location CPP");
}

EBTNodeResult::Type UCPP_BTTask_FindPlayerLocation_CB::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	FVector Location = PlayerController->GetPawn()->GetActorLocation();

	AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location);

	//FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
