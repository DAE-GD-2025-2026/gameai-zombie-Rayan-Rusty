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
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	UBlackboardComponent* Board = OwnerComp.GetBlackboardComponent();
	if (!Board) return EBTNodeResult::Failed;

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(Controller->GetPawn());
	if (!Survivor) return EBTNodeResult::Failed;

	ABaseZombie* Zombie = Cast<ABaseZombie>(Board->GetValueAsObject(FName("Zombie")));
	if (!Zombie) return EBTNodeResult::Failed;

	UInventoryComponent* Inventory = Survivor->FindComponentByClass<UInventoryComponent>();
	if (!Inventory) return EBTNodeResult::Failed;


	ABaseItem* WeaponToUse = nullptr;
	int WeaponSlotIndex = -1;

	
	
	
	
	
	TArray<ABaseItem*> const& CurrentInventory = Inventory->GetInventory();
	for (int i = 0; i < CurrentInventory.Num(); i++)
	{
		if (Cast<APistol>(CurrentInventory[i]) || Cast<AShotgun>(CurrentInventory[i]))
		{
			if (CurrentInventory[i]->GetValue() <= 0)
			{
				Inventory->RemoveItem(i);
				continue;
			}
			
			WeaponToUse = CurrentInventory[i];
			WeaponSlotIndex = i;
			break;
		}
	}

	if (!WeaponToUse)
	{
		Board->SetValueAsBool(FName("HasWeapon"), false);
		return EBTNodeResult::Failed;
	}

	

	float DistToZombie = FVector::Dist(Survivor->GetActorLocation(), Zombie->GetActorLocation());
	if (DistToZombie > 100.f)
	{
		Controller->MoveToActor(Zombie, 50.f, false);
		return EBTNodeResult::Succeeded;
	}

	FVector Direction = (Zombie->GetActorLocation() - Survivor->GetActorLocation()).GetSafeNormal();

	Controller->SetControlRotation(Direction.Rotation());
	Inventory->UseItem(WeaponSlotIndex);




	bool bStillHasWeapon = false;
	for (ABaseItem* Item : CurrentInventory)
	{
		if (Cast<APistol>(Item) || Cast<AShotgun>(Item))
		{
			bStillHasWeapon = true;
			break;
		}
	}
	Board->SetValueAsBool(FName("HasWeapon"), bStillHasWeapon);

	return EBTNodeResult::Succeeded;
}
