// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_RunAwayFromZombie.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
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

	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Blue, 
FString::Printf(TEXT("Enemy nearby run away!")));
	
	FVector FleeDirection = (survivor->GetActorLocation() - Zombie->GetActorLocation()).GetSafeNormal();
	FVector FleeTarget = survivor->GetActorLocation() + FleeDirection * 500.f;

	
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(survivor->GetWorld());
	FNavLocation NavLocation;
	if (NavSys && NavSys->GetRandomReachablePointInRadius(FleeTarget, 200.f, NavLocation))
	{
		FleeTarget = NavLocation.Location;
	}

	Controller->MoveToLocation(FleeTarget, 50.f, false);

	return EBTNodeResult::Succeeded;

}
