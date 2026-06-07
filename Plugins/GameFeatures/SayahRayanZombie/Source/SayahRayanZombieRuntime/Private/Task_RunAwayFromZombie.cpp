// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_RunAwayFromZombie.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PurgeZones/PurgeZone.h"
#include "Survivor/SurvivorPawn.h"
#include "Zombies/BaseZombie.h"

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
	
	ABaseZombie* Zombie = Cast<ABaseZombie>(board->GetValueAsObject(FName("Zombie")));
	if (!Zombie) return EBTNodeResult::Failed;

	
	const float DistZombie = FVector::Dist(survivor->GetActorLocation() , Zombie->GetActorLocation());

	if (DistZombie > 1500.f)
	{
		board->SetValueAsObject(FName("Zombie") , nullptr);
		survivor->StopRunning();
		return EBTNodeResult::Succeeded;
	}

	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Blue, 
FString::Printf(TEXT("Enemy nearby run away!")));
	
	FVector FleeDirection = (survivor->GetActorLocation() - Zombie->GetActorLocation()).GetSafeNormal();


	float FleeYaw = FMath::Atan2(FleeDirection.Y, FleeDirection.X);
	FleeAngle = FMath::Lerp(FleeAngle, FleeYaw, 0.3f); // gradually steer toward flee direction
	FleeAngle += FMath::DegreesToRadians(FMath::RandRange(-MaxAngleChange, MaxAngleChange)); // keep some randomness

	FVector2D SurvivorPos(survivor->GetActorLocation().X, survivor->GetActorLocation().Y);
	FVector2D Forward(survivor->GetActorForwardVector().X, survivor->GetActorForwardVector().Y);
	FVector2D CircleCenter = SurvivorPos + Forward * OffsetDistance;

	FVector2D WanderTarget(
		CircleCenter.X + FMath::Cos(FleeAngle) * FleeRadius,
		CircleCenter.Y + FMath::Sin(FleeAngle) * FleeRadius
	);

	FVector FleeTarget(WanderTarget.X, WanderTarget.Y, survivor->GetActorLocation().Z);
	
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(survivor->GetWorld());
	if (NavSys)
	{
		FNavLocation NavLocation;
		if (NavSys->ProjectPointToNavigation(FleeTarget, NavLocation, FVector(100.f, 100.f, 100.f)))
		{
			FleeTarget = NavLocation.Location;
		}
		else
		{
			FNavLocation FallbackLocation;
			if (NavSys->GetRandomReachablePointInRadius(survivor->GetActorLocation(), FleeRadius, FallbackLocation))
				FleeTarget = FallbackLocation.Location;
		}
	}


	
	if (IsInsidePurgeZone(FleeTarget , board ))
	{
		survivor->StopRunning();
		return EBTNodeResult::Failed;
	}

	
	if (DistZombie < 500.f)
	{
		survivor->StartRunning();	
	}
	
	
	Controller->MoveToLocation(FleeTarget, 50.f, false);

	return EBTNodeResult::Succeeded;

}

bool UTask_RunAwayFromZombie::IsInsidePurgeZone(const FVector Location, UBlackboardComponent* Board) const
{
	APurgeZone* PurgeZone = Cast<APurgeZone>(Board->GetValueAsObject(FName("PurgeZone")));
	if (!PurgeZone) return false;
	
	return PurgeZone->GetRootComponent()->Bounds.GetBox().IsInside(Location);
}
