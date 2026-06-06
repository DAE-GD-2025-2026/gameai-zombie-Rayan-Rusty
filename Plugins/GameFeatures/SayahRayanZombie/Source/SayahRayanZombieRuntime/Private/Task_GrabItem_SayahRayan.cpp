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
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	UBlackboardComponent* Board = OwnerComp.GetBlackboardComponent();
	if (!Board) return EBTNodeResult::Failed;

	APawn* Pawn = Controller->GetPawn();
	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(Pawn);
	if (!Survivor) return EBTNodeResult::Failed;
	
	UHealthComponent* HealthComp = Survivor->FindComponentByClass<UHealthComponent>();
	UStaminaComponent* StaminaComp = Survivor->FindComponentByClass<UStaminaComponent>();
	
	ABaseItem* ItemToGrab = nullptr;

	if (!ItemToGrab)
	{
		if (AMedkit* Medkit = Cast<AMedkit>(Board->GetValueAsObject(FName("Medkit"))))
			ItemToGrab = Medkit;
		else if (AFood* Food = Cast<AFood>(Board->GetValueAsObject(FName("Food"))))
			ItemToGrab = Food;
		else if (APistol* Pistol = Cast<APistol>(Board->GetValueAsObject(FName("Handgun"))))
			ItemToGrab = Pistol;
		else if (AShotgun* Shotgun = Cast<AShotgun>(Board->GetValueAsObject(FName("Shotgun"))))
			ItemToGrab = Shotgun;
		

	}

	if (!ItemToGrab) return EBTNodeResult::Failed;
	
	float Dist = FVector::Dist(Survivor->GetActorLocation(), ItemToGrab->GetActorLocation());

	UInventoryComponent* Inventory = Survivor->FindComponentByClass<UInventoryComponent>();
	if (!Inventory) return EBTNodeResult::Failed;

	int EmptySlotIndex = -1;
	TArray<ABaseItem*> const& CurrentInventory = Inventory->GetInventory();
	for (int i = 0; i < CurrentInventory.Num(); i++)
	{
		if (CurrentInventory[i] == nullptr)
		{
			EmptySlotIndex = i;
			break;
		}
	}

	if (EmptySlotIndex == -1)
	{
		GEngine->AddOnScreenDebugMessage(6, 2.f, FColor::Red, TEXT("GrabItem FAILED: No empty slot"));
		return EBTNodeResult::Failed;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Attempting grab: Slot=%d, Item=%s, Dist=%.1f"),
	EmptySlotIndex, *ItemToGrab->GetName(), Dist);

	if (Inventory->GrabItem(EmptySlotIndex, ItemToGrab))
	{
		if (Cast<AFood>(ItemToGrab))         Board->SetValueAsObject(FName("Food"), nullptr);
		else if (Cast<AMedkit>(ItemToGrab))  Board->SetValueAsObject(FName("Medkit"), nullptr);
		else if (Cast<APistol>(ItemToGrab))  Board->SetValueAsObject(FName("Handgun"), nullptr);
		else if (Cast<AShotgun>(ItemToGrab)) Board->SetValueAsObject(FName("Shotgun"), nullptr);

		Board->SetValueAsBool(FName("ItemSeen") , false);
		
		if (Cast<APistol>(ItemToGrab) || Cast<AShotgun>(ItemToGrab))
			Board->SetValueAsBool(FName("HasWeapon"), true);
		
		bool NowFull {true};
		for (ABaseItem* Item :CurrentInventory)
		{
			if (Item == nullptr) { NowFull = false; }
		}
		Board->SetValueAsBool(FName("IsInventoryFull"), NowFull);
		return EBTNodeResult::Succeeded;
	}
	else
	{

	}
	


	return EBTNodeResult::Failed;
}
