// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Items/BaseItem.h"
#include "Task_GrabItem_SayahRayan.generated.h"

/**
 * 
 */
UCLASS()
class SAYAHRAYANZOMBIERUNTIME_API UTask_GrabItem_SayahRayan : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTask_GrabItem_SayahRayan();
	virtual  EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	ASurvivorPawn* GetSurvivor(UBehaviorTreeComponent& OwnerComp) const;
	ABaseItem* ResolvePriorityItem(UBlackboardComponent* Board) const;
	int32 FindEmptySlot(UInventoryComponent* Inventory) const;
	void ClearItemFromBlackboard(UBlackboardComponent* Board, const ABaseItem* Item) const;
	void UpdateBlackboardPostGrab(UBlackboardComponent* Board, const ABaseItem* GrabbedItem, UInventoryComponent* Inventory) const;
	bool TryDropLowestPriorityItem(UInventoryComponent* Inventory, UBlackboardComponent* Board) const;
};
