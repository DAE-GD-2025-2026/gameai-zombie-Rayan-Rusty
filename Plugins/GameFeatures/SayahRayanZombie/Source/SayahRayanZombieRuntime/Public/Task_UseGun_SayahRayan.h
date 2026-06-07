// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Survivor/SurvivorPawn.h"
#include "Items/BaseItem.h"
#include "Common/InventoryComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Task_UseGun_SayahRayan.generated.h"

/**
 * 
 */
UCLASS()
class SAYAHRAYANZOMBIERUNTIME_API UTask_UseGun_SayahRayan : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTask_UseGun_SayahRayan();
	virtual  EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	ASurvivorPawn* GetSurvivor(UBehaviorTreeComponent& OwnerComp) const;
	TPair<ABaseItem*, int32> FindLowestAmmoWeapon(UInventoryComponent* Inventory) const;
	void UpdateHasWeapon(UInventoryComponent* Inventory, UBlackboardComponent* Board) const;
};
