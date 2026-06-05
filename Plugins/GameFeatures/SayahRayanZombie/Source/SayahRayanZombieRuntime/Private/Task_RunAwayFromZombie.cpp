// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_RunAwayFromZombie.h"

#include "AIController.h"
#include "Survivor/SurvivorPawn.h"

UTask_RunAwayFromZombie::UTask_RunAwayFromZombie()
{
	NodeName = "Run Away From Zombie";
}

EBTNodeResult::Type UTask_RunAwayFromZombie::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;
	
	ASurvivorPawn* survivor = Cast<ASurvivorPawn>(Controller->GetPawn());
	if (!survivor) return EBTNodeResult::Failed;
	
	UBlackboardComponent* board = OwnerComp.GetBlackboardComponent();
	if (!board) return EBTNodeResult::Failed;
	
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
