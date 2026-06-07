// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Common/InventoryComponent.h"
#include "Task_MoveToItem_SayahRayan.generated.h"

/**
 * 
 */
UCLASS()
class SAYAHRAYANZOMBIERUNTIME_API UTask_MoveToItem_SayahRayan : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTask_MoveToItem_SayahRayan();
	virtual  EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	ASurvivorPawn*  GetSurvivor(UBehaviorTreeComponent& OwnerComp) const;
	ABaseItem*      ResolvePriorityItem(UBlackboardComponent* Board, bool StaminaMoreUrgent) const;
	bool            IsInventoryFull(UInventoryComponent* Inventory) const;
	bool            TryDropLowestPriorityItem(UInventoryComponent* Inventory, UBlackboardComponent* Board) const;
	void            MoveToItem(AAIController* Controller, ASurvivorPawn* Survivor, ABaseItem* TargetItem) const;
	float           GetStatPercent(float Current, float Max) const;
};
