// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_MoveToHouse_SayahRayan.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Village/House/House.h"

UTask_MoveToHouse_SayahRayan::UTask_MoveToHouse_SayahRayan()
{
	NodeName = ("Move To House");
}

EBTNodeResult::Type UTask_MoveToHouse_SayahRayan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	UBlackboardComponent* Board = OwnerComp.GetBlackboardComponent();
	if (!Board) return EBTNodeResult::Failed;

	AHouse* House = Cast<AHouse>(Board->GetValueAsObject(FName("House")));
	if (!House) return EBTNodeResult::Failed;

	Controller->MoveToActor(House, 50.f, false);

	return EBTNodeResult::Succeeded;
}
