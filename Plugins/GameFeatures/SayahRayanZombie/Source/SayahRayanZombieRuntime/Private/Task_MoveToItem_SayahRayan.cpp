// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_MoveToItem_SayahRayan.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Items/BaseItem.h"
#include "Items/Food.h"
#include "Items/Medkit.h"
#include "Items/Pistol.h"
#include "Items/Shotgun.h"
#include "Survivor/SurvivorPawn.h"

UTask_MoveToItem_SayahRayan::UTask_MoveToItem_SayahRayan()
{
}

EBTNodeResult::Type UTask_MoveToItem_SayahRayan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//TODO empty gun , check inventory fix snapping by using project navmesh
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	UBlackboardComponent* Board = OwnerComp.GetBlackboardComponent();
	if (!Board) return EBTNodeResult::Failed;

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(Controller->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;

	UHealthComponent* HealthComp = Survivor->FindComponentByClass<UHealthComponent>();
	UStaminaComponent* StaminaComp = Survivor->FindComponentByClass<UStaminaComponent>();


	float HealthPercent =  HealthComp->GetHealth() / HealthComp->GetMaxHealth();
	float StaminePercent = StaminaComp->GetCurrentStamina() / StaminaComp->GetMaxStamina();

	bool StaminaMoreUrgent = StaminePercent < HealthPercent;
	ABaseItem* TargetItem = nullptr;


	if (StaminaMoreUrgent)
	{
		if (AFood* Food = Cast<AFood>(Board->GetValueAsObject(FName("Food"))))
		{
			TargetItem = Food;
		}
		else if (AMedkit* Medkit = Cast<AMedkit>(Board->GetValueAsObject(FName("Medkit"))))
		{
			TargetItem = Medkit;
		}
	}
	else
	{
		if (AMedkit* Medkit = Cast<AMedkit>(Board->GetValueAsObject(FName("Medkit"))))
		{
			TargetItem = Medkit;
		}
		else if(AFood* Food = Cast<AFood>(Board->GetValueAsObject(FName("Food"))))
		{
			TargetItem = Food;
		}
	}

	if (!TargetItem)
	{
		if (APistol* Pistol = Cast<APistol>(Board->GetValueAsObject(FName("Handgun"))))
			TargetItem = Pistol;
		else if (AShotgun* Shotgun = Cast<AShotgun>(Board->GetValueAsObject(FName("Shotgun"))))
			TargetItem = Shotgun;
	}

	
	if (!TargetItem) return EBTNodeResult::Failed;

	float Dist = FVector::Dist(Survivor->GetActorLocation(), TargetItem->GetActorLocation());

	if (Dist > 100.f)
	{
		FVector TargetLocation = TargetItem->GetActorLocation();

		// Project onto navmesh
		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Survivor->GetWorld());
		FNavLocation NavLocation;
		if (NavSys && NavSys->ProjectPointToNavigation(TargetLocation, NavLocation))
		{
			TargetLocation = NavLocation.Location;
		}

		Controller->MoveToLocation(TargetLocation, 20.f, false);
		return EBTNodeResult::Succeeded;
	}

	

	return EBTNodeResult::Succeeded;

}
