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


	float MissingHealth = HealthComp->GetMaxHealth() - HealthComp->GetHealth();
	float MissingStamina = StaminaComp->GetMaxStamina() - StaminaComp->GetCurrentStamina();
	if (MissingHealth <= 0.f && MissingStamina <= 0.f) return EBTNodeResult::Failed;
	
	float HealthPercent = HealthComp->GetHealth() / HealthComp->GetMaxHealth();
	float StaminaPercent = StaminaComp->GetCurrentStamina() / StaminaComp->GetMaxStamina();
	bool bStaminaMoreUrgent = StaminaPercent < HealthPercent;
	
	TArray<ABaseItem*> const& CurrentInventory = Inventory->GetInventory();

	int BestItemIndex {-1};
	float BestDiff = FLT_MAX;
	
	for (int i = 0;  i < CurrentInventory.Num() ; ++i)
	{
		if (!CurrentInventory[i]) continue;
		
		float Missing = 0.f;
		float Max = 1.f;

		if (CurrentInventory[i]->GetItemType() == EItemType::Medkit)
		{
			if (bStaminaMoreUrgent) continue; 
			Missing = MissingHealth;
			Max = HealthComp->GetMaxHealth();
		}
		else if (CurrentInventory[i]->GetItemType() == EItemType::Food)
		{
			if (!bStaminaMoreUrgent) continue; 
			Missing = MissingStamina;
			Max = StaminaComp->GetMaxStamina();
		}
		else continue;

		float ItemValue = static_cast<float>(CurrentInventory[i]->GetValue());
		if (ItemValue > Missing) continue;
		
		float Diff = FMath::Abs(Missing - CurrentInventory[i]->GetValue());
		if (Diff < BestDiff)
		{
			BestDiff = Diff;
			BestItemIndex = i;
		}
	}
	
	if (BestItemIndex == -1) return EBTNodeResult::Failed;

	Inventory->UseItem(BestItemIndex);
	Inventory->RemoveItem(BestItemIndex);
	return EBTNodeResult::Succeeded;
}
