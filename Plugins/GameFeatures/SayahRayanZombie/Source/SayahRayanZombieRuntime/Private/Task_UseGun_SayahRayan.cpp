// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_UseGun_SayahRayan.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Items/Pistol.h"
#include "Items/Shotgun.h"
#include "Zombies/BaseZombie.h"

UTask_UseGun_SayahRayan::UTask_UseGun_SayahRayan()
{
	NodeName = ("USe Gun");
}

EBTNodeResult::Type UTask_UseGun_SayahRayan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Board = OwnerComp.GetBlackboardComponent();
	if (!Board) return EBTNodeResult::Failed;

	ASurvivorPawn* Survivor = GetSurvivor(OwnerComp);
	if (!Survivor) return EBTNodeResult::Failed;

	ABaseZombie* Zombie = Cast<ABaseZombie>(Board->GetValueAsObject(FName("Zombie")));
	if (!Zombie) return EBTNodeResult::Failed;

	UInventoryComponent* Inventory = Survivor->FindComponentByClass<UInventoryComponent>();
	if (!Inventory) return EBTNodeResult::Failed;

	auto [WeaponToUse, WeaponSlot] = FindLowestAmmoWeapon(Inventory);
	if (!WeaponToUse)
	{
		Board->SetValueAsBool(FName("HasWeapon"), false);
		return EBTNodeResult::Failed;
	}

	const float DistToZombie = FVector::Dist(Survivor->GetActorLocation(), Zombie->GetActorLocation());
	if (DistToZombie > 100.f)
	{
		OwnerComp.GetAIOwner()->MoveToActor(Zombie, 50.f, false);
		return EBTNodeResult::Succeeded;
	}

	const FVector Direction = (Zombie->GetActorLocation() - Survivor->GetActorLocation()).GetSafeNormal();
	OwnerComp.GetAIOwner()->SetControlRotation(Direction.Rotation());

	Inventory->UseItem(WeaponSlot);

	// drop the weapon if its now empty !! 
	if (WeaponToUse->GetValue() <= 0)
		Inventory->RemoveItem(WeaponSlot);

	UpdateHasWeapon(Inventory, Board);
	return EBTNodeResult::Succeeded;
}

ASurvivorPawn* UTask_UseGun_SayahRayan::GetSurvivor(UBehaviorTreeComponent& OwnerComp) const
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	return Controller ? Cast<ASurvivorPawn>(Controller->GetPawn()) : nullptr;
}

TPair<ABaseItem*, int32> UTask_UseGun_SayahRayan::FindLowestAmmoWeapon(UInventoryComponent* Inventory) const
{
	const TArray<ABaseItem*>& Items = Inventory->GetInventory();

	ABaseItem* BestWeapon    = nullptr;
	int32      BestSlot      = INDEX_NONE;
	int32      LowestAmmo    = TNumericLimits<int32>::Max();

	for (int32 i = 0; i < Items.Num(); ++i)
	{
		ABaseItem* Item = Items[i];
		if (!Item || !Item->IsA<APistol>() && !Item->IsA<AShotgun>()) continue;

		// guns with no ammo get removed since they take up space!!!
		if (Item->GetValue() <= 0)
		{
			Inventory->RemoveItem(i);
			continue;
		}

		if (Item->GetValue() < LowestAmmo)
		{
			LowestAmmo  = Item->GetValue();
			BestWeapon  = Item;
			BestSlot    = i;
		}
	}

	return { BestWeapon, BestSlot };
}

void UTask_UseGun_SayahRayan::UpdateHasWeapon(UInventoryComponent* Inventory, UBlackboardComponent* Board) const
{
	const bool StillHasWeapon = Inventory->GetInventory().ContainsByPredicate(
	[](const ABaseItem* Item) { return Item && (Item->IsA<APistol>() || Item->IsA<AShotgun>()); });

	Board->SetValueAsBool(FName("HasWeapon"), StillHasWeapon);
}
