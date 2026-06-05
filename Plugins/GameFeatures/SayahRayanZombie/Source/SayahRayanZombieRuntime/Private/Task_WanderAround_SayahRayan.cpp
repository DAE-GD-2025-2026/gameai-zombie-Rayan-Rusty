// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_WanderAround_SayahRayan.h"

#include "AIController.h"
#include "Survivor/SurvivorPawn.h"

UTask_WanderAround_SayahRayan::UTask_WanderAround_SayahRayan()
{
	NodeName = "Wander Around";
	bNotifyTick = true;
}

EBTNodeResult::Type UTask_WanderAround_SayahRayan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	APawn* Pawn = Controller->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;
	
	
	FVector PawnLocation = Pawn->GetActorLocation();
	FVector2D Position2D(PawnLocation.X, PawnLocation.Y);

	FVector2D Forward(Pawn->GetActorForwardVector().X, Pawn->GetActorForwardVector().Y);
	FVector2D CircleCenter = Position2D + Forward * OffsetDistance;

	WanderAngle += FMath::DegreesToRadians(FMath::RandRange(-MaxAngleChange, MaxAngleChange));

	FVector2D WanderTarget(
		CircleCenter.X + FMath::Cos(WanderAngle) * WanderRadius,
		CircleCenter.Y + FMath::Sin(WanderAngle) * WanderRadius
	);

	FVector TargetLocation(WanderTarget.X, WanderTarget.Y, PawnLocation.Z);

	Controller->MoveToLocation(TargetLocation, AcceptanceRadius, false);

	return EBTNodeResult::Succeeded;

}
