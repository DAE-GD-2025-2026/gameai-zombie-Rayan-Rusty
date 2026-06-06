// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Village/House/House.h"
#include "Task_MoveToHouse_SayahRayan.generated.h"

/**
 * 
 */
UCLASS()
class SAYAHRAYANZOMBIERUNTIME_API UTask_MoveToHouse_SayahRayan : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTask_MoveToHouse_SayahRayan();
	virtual  EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	TSet<AHouse*> VisitedHouses;
	bool movingToHouse {false};
};
