// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_RotateToFind_SayahRayan.h"

#include "AIController.h"
#include "Survivor/SurvivorPawn.h"

UTask_RotateToFind_SayahRayan::UTask_RotateToFind_SayahRayan()
{
	NodeName = "Rotate Around";
	bNotifyTick = true;
}

void UTask_RotateToFind_SayahRayan::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	ASurvivorPawn* Survivor = GetSurvivor(OwnerComp);
	if (!Survivor)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const FRotator CurrentRotation = Survivor->GetActorRotation();
	Survivor->SetActorRotation(FRotator(
		CurrentRotation.Pitch,
		CurrentRotation.Yaw + RotationStepDegrees * DeltaSeconds,
		CurrentRotation.Roll
	)); 
	
}



ASurvivorPawn* UTask_RotateToFind_SayahRayan::GetSurvivor(const UBehaviorTreeComponent& OwnerComp)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	return Controller ? Cast<ASurvivorPawn>(Controller->GetPawn()) : nullptr;
}
