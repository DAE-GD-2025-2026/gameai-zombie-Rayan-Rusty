// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_WanderAround_SayahRayan.generated.h"

/**
 * 
 */
UCLASS()
class SAYAHRAYANZOMBIERUNTIME_API UTask_WanderAround_SayahRayan : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTask_WanderAround_SayahRayan();
	virtual  EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
