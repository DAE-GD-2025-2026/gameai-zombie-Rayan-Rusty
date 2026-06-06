// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_MoveToHouse_SayahRayan.h"

#include "AIController.h"
#include "NavigationSystem.h"
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

	
	GEngine->AddOnScreenDebugMessage(5
		, 1.f, FColor::Green, FString::Printf(TEXT("HouseInside!")));
	UBlackboardComponent* Board = OwnerComp.GetBlackboardComponent();
	if (!Board) return EBTNodeResult::Failed;

	AHouse* House = Cast<AHouse>(Board->GetValueAsObject(FName("House")));
	if (!House) return EBTNodeResult::Failed;

	if (VisitedHouses.Contains(House))
	{
		Board->SetValueAsObject(FName("House") , nullptr);
		return EBTNodeResult::Failed;
	}
	
	float dist = FVector::Dist(Controller->GetPawn()->GetActorLocation() , House->GetActorLocation());
	
	if (dist > 200.f)
	{

		FVector TargetLocation = House->GetActorLocation();

		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Controller->GetPawn()->GetWorld());
		FNavLocation NavLocation;
		if (NavSys && NavSys->ProjectPointToNavigation(TargetLocation, NavLocation))
		{
			TargetLocation = NavLocation.Location;
		}


		Controller->MoveToLocation(TargetLocation, 50.f, false);
		return EBTNodeResult::Succeeded;
	}


	
	VisitedHouses.Add(House);
	return EBTNodeResult::Succeeded;
}
