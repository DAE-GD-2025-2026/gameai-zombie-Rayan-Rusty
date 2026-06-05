// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_UseItem_SayahRayan.h"

#include "AIController.h"
#include "Common/InventoryComponent.h"
#include "Items/Food.h"
#include "Items/Medkit.h"

UTask_UseItem_SayahRayan::UTask_UseItem_SayahRayan()
{
	NodeName = ("Use Item");
}

EBTNodeResult::Type UTask_UseItem_SayahRayan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	UBlackboardComponent* Board = OwnerComp.GetBlackboardComponent();
	if (!Board) return EBTNodeResult::Failed;

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(Controller->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;

	UHealthComponent* HealthComp = Survivor->FindComponentByClass<UHealthComponent>();
	UStaminaComponent* StaminaComp = Survivor->FindComponentByClass<UStaminaComponent>();
	UInventoryComponent* Inventory = Survivor->FindComponentByClass<UInventoryComponent>();
	if (!Inventory) return EBTNodeResult::Failed;

	bool bNeedsHealth = HealthComp && HealthComp->GetHealth() < 50.f;
	bool bNeedsStamina = StaminaComp && StaminaComp->GetCurrentStamina() < 50.f;

	TArray<ABaseItem*> const& CurrentInventory = Inventory->GetInventory();

	for (int i = 0; i < CurrentInventory.Num(); i++)
	{
		if (bNeedsHealth && Cast<AMedkit>(CurrentInventory[i]))
		{
			Inventory->UseItem(i);
			return EBTNodeResult::Succeeded;
		}
		if (bNeedsStamina && Cast<AFood>(CurrentInventory[i]))
		{
			Inventory->UseItem(i);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
