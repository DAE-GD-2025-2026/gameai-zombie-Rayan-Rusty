// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_WanderAround_SayahRayan.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PurgeZones/PurgeZone.h"
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
	
	UBlackboardComponent* board = OwnerComp.GetBlackboardComponent();
	if (!board) return EBTNodeResult::Failed;

	ASurvivorPawn* survivor = Cast<ASurvivorPawn>(Controller->GetPawn());
	if (!survivor) return EBTNodeResult::Failed;
	
	survivor->StopRunning();
	
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


	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn->GetWorld());
	if (NavSys)
	{
		FNavLocation NavLocation;
		if (NavSys->ProjectPointToNavigation(TargetLocation, NavLocation, FVector(100.f, 100.f, 100.f)))
		{
			TargetLocation = NavLocation.Location;
		}
		else
		{

			FNavLocation FallbackLocation;
			if (NavSys->GetRandomReachablePointInRadius(PawnLocation, WanderRadius, FallbackLocation))
				TargetLocation = FallbackLocation.Location;
		}
	}

	if (IsInsidePurgeZone(TargetLocation , board ))
		return EBTNodeResult::Failed;
	
	Controller->MoveToLocation(TargetLocation, AcceptanceRadius, false);
	return EBTNodeResult::Succeeded;

}

bool UTask_WanderAround_SayahRayan::IsInsidePurgeZone(const FVector Location, UBlackboardComponent* Board) const
{
	APurgeZone* PurgeZone = Cast<APurgeZone>(Board->GetValueAsObject(FName("PurgeZone")));
	if (!PurgeZone) return false;
	
	return PurgeZone->GetRootComponent()->Bounds.GetBox().IsInside(Location);
}
