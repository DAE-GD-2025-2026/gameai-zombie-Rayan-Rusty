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
	UBlackboardComponent* Board = OwnerComp.GetBlackboardComponent();
	if (!Board) return EBTNodeResult::Failed;

	ASurvivorPawn* Survivor = GetSurvivor(OwnerComp);
	if (!Survivor) return EBTNodeResult::Failed;

	Survivor->StopRunning();
	UHealthComponent*  HealthComp  = Survivor->FindComponentByClass<UHealthComponent>();
	UStaminaComponent* StaminaComp = Survivor->FindComponentByClass<UStaminaComponent>();
	if (!HealthComp || !StaminaComp) return EBTNodeResult::Failed;

	const float HealthPercent     = GetStatPercent(HealthComp->GetHealth(), HealthComp->GetMaxHealth());
	const float StaminaPercent    = GetStatPercent(StaminaComp->GetCurrentStamina(), StaminaComp->GetMaxStamina());
	const bool  StaminaMoreUrgent = StaminaPercent < HealthPercent;

	ABaseItem* TargetItem = ResolvePriorityItem(Board, StaminaMoreUrgent);
	if (!TargetItem) return EBTNodeResult::Failed;

	MoveToItem(OwnerComp.GetAIOwner(), Survivor, TargetItem);
	return EBTNodeResult::Succeeded;
}

ASurvivorPawn* UTask_MoveToItem_SayahRayan::GetSurvivor(UBehaviorTreeComponent& OwnerComp) const
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	return Controller ? Cast<ASurvivorPawn>(Controller->GetPawn()) : nullptr;
}

ABaseItem* UTask_MoveToItem_SayahRayan::ResolvePriorityItem(UBlackboardComponent* Board, bool StaminaMoreUrgent) const
{
	const TArray<FName> ConsumableOrder = StaminaMoreUrgent
	? TArray<FName>{ FName("Food"), FName("Medkit") }
	: TArray<FName>{ FName("Medkit"), FName("Food") };

	for (const FName& Key : ConsumableOrder)
	{
		if (ABaseItem* Item = Cast<ABaseItem>(Board->GetValueAsObject(Key)))
			return Item;
	}

	// Fall back to weapons if no consumables available
	const TArray<FName> WeaponOrder = { FName("Handgun"), FName("Shotgun") };
	for (const FName& Key : WeaponOrder)
	{
		if (ABaseItem* Item = Cast<ABaseItem>(Board->GetValueAsObject(Key)))
			return Item;
	}

	return nullptr;
}

bool UTask_MoveToItem_SayahRayan::IsInventoryFull(UInventoryComponent* Inventory) const
{
	return Inventory->GetInventory().FindByPredicate(
	[](const ABaseItem* Item)
	{
		return Item == nullptr;
	}
	) == nullptr;
}



void UTask_MoveToItem_SayahRayan::MoveToItem(AAIController* Controller, ASurvivorPawn* Survivor,
	ABaseItem* TargetItem) const
{
	const float Dist = FVector::Dist(Survivor->GetActorLocation(), TargetItem->GetActorLocation());
	if (Dist <= 100.f) return;

	FVector TargetLocation = TargetItem->GetActorLocation();

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Survivor->GetWorld());
	FNavLocation NavLocation;
	if (NavSys && NavSys->ProjectPointToNavigation(TargetLocation, NavLocation))
		TargetLocation = NavLocation.Location;

	Controller->MoveToLocation(TargetLocation, 20.f, false);
}

float UTask_MoveToItem_SayahRayan::GetStatPercent(float Current, float Max) const
{
	return Max > 0.f ? Current / Max : 0.f;
}
