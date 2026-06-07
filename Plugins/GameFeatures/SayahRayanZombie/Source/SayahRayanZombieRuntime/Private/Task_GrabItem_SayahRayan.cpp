// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_GrabItem_SayahRayan.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/InventoryComponent.h"
#include "Items/Food.h"
#include "Items/Medkit.h"
#include "Items/Pistol.h"
#include "Items/Shotgun.h"
#include "Survivor/SurvivorPawn.h"

UTask_GrabItem_SayahRayan::UTask_GrabItem_SayahRayan()
{
	NodeName = ("Grab Item");
}

EBTNodeResult::Type UTask_GrabItem_SayahRayan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Board = OwnerComp.GetBlackboardComponent();
	if (!Board) return EBTNodeResult::Failed;

	ASurvivorPawn* Survivor = GetSurvivor(OwnerComp);
	if (!Survivor) return EBTNodeResult::Failed;

	ABaseItem* ItemToGrab = ResolvePriorityItem(Board);
	if (!ItemToGrab) return EBTNodeResult::Failed;

	const float Dist = FVector::Dist(Survivor->GetActorLocation(), ItemToGrab->GetActorLocation());



	UInventoryComponent* Inventory = Survivor->FindComponentByClass<UInventoryComponent>();
	if (!Inventory) return EBTNodeResult::Failed;
	if (FindEmptySlot(Inventory) == INDEX_NONE)
	{
		const bool bNeedsConsumable = ItemToGrab->IsA<AFood>() || ItemToGrab->IsA<AMedkit>();
		if (!bNeedsConsumable || !TryDropLowestPriorityItem(Inventory, Board))
		{
			GEngine->AddOnScreenDebugMessage(6, 2.f, FColor::Red, TEXT("GrabItem FAILED: No empty slot"));
			return EBTNodeResult::Failed;
		}
	}

	const int32 EmptySlot = FindEmptySlot(Inventory);
	if (EmptySlot == INDEX_NONE) return EBTNodeResult::Failed;

	UE_LOG(LogTemp, Warning, TEXT("Attempting grab: Slot=%d, Item=%s, Dist=%.1f"),
		EmptySlot, *ItemToGrab->GetName(),
		FVector::Dist(Survivor->GetActorLocation(), ItemToGrab->GetActorLocation()));

	if (!Inventory->GrabItem(EmptySlot, ItemToGrab))
		return EBTNodeResult::Failed;

	UpdateBlackboardPostGrab(Board, ItemToGrab, Inventory);
	return EBTNodeResult::Succeeded;
}

ASurvivorPawn* UTask_GrabItem_SayahRayan::GetSurvivor(UBehaviorTreeComponent& OwnerComp) const
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	return Controller ? Cast<ASurvivorPawn>(Controller->GetPawn()) : nullptr;
}

ABaseItem* UTask_GrabItem_SayahRayan::ResolvePriorityItem(UBlackboardComponent* Board) const
{
	const TArray<TPair<FName, TSubclassOf<ABaseItem>>> Priority =
	{
		{ FName("Medkit"),   AMedkit::StaticClass()  },
		{ FName("Food"),     AFood::StaticClass()    },
		{ FName("Handgun"),  APistol::StaticClass()  },
		{ FName("Shotgun"),  AShotgun::StaticClass() },
	};

	for (const auto& [Key, Class] : Priority)
	{
		if (ABaseItem* Item = Cast<ABaseItem>(Board->GetValueAsObject(Key)))
			return Item;
	}
	return nullptr;
}

int32 UTask_GrabItem_SayahRayan::FindEmptySlot(UInventoryComponent* Inventory) const
{
	const TArray<ABaseItem*>& Items = Inventory->GetInventory();
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i] == nullptr) return i;
	}
	return INDEX_NONE;
}

void UTask_GrabItem_SayahRayan::ClearItemFromBlackboard(UBlackboardComponent* Board, const ABaseItem* Item) const
{
	const TArray<TPair<FName, TSubclassOf<ABaseItem>>> KeyMap =
	{
		{ FName("Medkit"),   AMedkit::StaticClass()  },
		{ FName("Food"),     AFood::StaticClass()    },
		{ FName("Handgun"),  APistol::StaticClass()  },
		{ FName("Shotgun"),  AShotgun::StaticClass() },
	};

	for (const auto& [Key, Class] : KeyMap)
	{
		if (Item->IsA(Class))
		{
			Board->SetValueAsObject(Key, nullptr);
			return;
		}
	}
}

void UTask_GrabItem_SayahRayan::UpdateBlackboardPostGrab(UBlackboardComponent* Board, const ABaseItem* GrabbedItem,
	UInventoryComponent* Inventory) const
{
	ClearItemFromBlackboard(Board, GrabbedItem);

	Board->SetValueAsBool(FName("ItemSeen"), false);

	if (GrabbedItem->IsA<APistol>() || GrabbedItem->IsA<AShotgun>())
		Board->SetValueAsBool(FName("HasWeapon"), true);

	const bool bInventoryFull = Inventory->GetInventory().FindByPredicate(
		[](const ABaseItem* Item) { return Item == nullptr; }) == nullptr;
    
	Board->SetValueAsBool(FName("IsInventoryFull"), bInventoryFull);
}

bool UTask_GrabItem_SayahRayan::TryDropLowestPriorityItem(UInventoryComponent* Inventory, UBlackboardComponent* Board) const
{
	// Drop order is pistol then shotgun
	const TArray<TSubclassOf<ABaseItem>> DropOrder =
	{
		APistol::StaticClass(),
		AShotgun::StaticClass()
	};

	const TArray<ABaseItem*>& Items = Inventory->GetInventory();
	for (const TSubclassOf<ABaseItem>& Class : DropOrder)
	{
		for (int32 i = 0; i < Items.Num(); ++i)
		{
			if (Items[i] && Items[i]->IsA(Class))
			{
				const FString DroppedName = Items[i]->GetName();
				Inventory->RemoveItem(i);

				const bool StillHasWeapon = Items.ContainsByPredicate(
					[](const ABaseItem* Item) { return Item && (Item->IsA<APistol>() || Item->IsA<AShotgun>()); });
				Board->SetValueAsBool(FName("HasWeapon"), StillHasWeapon);

				UE_LOG(LogTemp , Warning , TEXT("Droppedweapon : %s tomake room for consumable") , *DroppedName);
				return true;
			}
		}
	}
	return false;
}

