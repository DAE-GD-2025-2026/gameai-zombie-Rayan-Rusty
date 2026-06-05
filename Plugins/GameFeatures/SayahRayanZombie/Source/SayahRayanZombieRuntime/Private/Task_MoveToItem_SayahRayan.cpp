// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_MoveToItem_SayahRayan.h"

#include "AIController.h"
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
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	UBlackboardComponent* Board = OwnerComp.GetBlackboardComponent();
	if (!Board) return EBTNodeResult::Failed;

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(Controller->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;

	UHealthComponent* HealthComp = Survivor->FindComponentByClass<UHealthComponent>();
	UStaminaComponent* StaminaComp = Survivor->FindComponentByClass<UStaminaComponent>();

	bool bNeedsHealth = HealthComp && HealthComp->GetHealth() < 50.f;
	bool bNeedsStamina = StaminaComp && StaminaComp->GetCurrentStamina() < 50.f;

	ABaseItem* TargetItem = nullptr;

	if (bNeedsHealth)
	{
		if (AMedkit* Medkit = Cast<AMedkit>(Board->GetValueAsObject(FName("Medkit"))))
			TargetItem = Medkit;
	}
	else if (bNeedsStamina)
	{
		if (AFood* Food = Cast<AFood>(Board->GetValueAsObject(FName("Food"))))
			TargetItem = Food;
	}

	if (!TargetItem)
	{
		if (AMedkit* Medkit = Cast<AMedkit>(Board->GetValueAsObject(FName("Medkit"))))
			TargetItem = Medkit;
		else if (AFood* Food = Cast<AFood>(Board->GetValueAsObject(FName("Food"))))
			TargetItem = Food;
		else if (APistol* Pistol = Cast<APistol>(Board->GetValueAsObject(FName("Handgun"))))
			TargetItem = Pistol;
		else if (AShotgun* Shotgun = Cast<AShotgun>(Board->GetValueAsObject(FName("Shotgun"))))
			TargetItem = Shotgun;
	}


	
	if (!TargetItem) return EBTNodeResult::Failed;

	Controller->MoveToActor(TargetItem, 1.f, false);
	return EBTNodeResult::Succeeded;
}
