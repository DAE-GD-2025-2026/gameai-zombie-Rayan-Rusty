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
	
protected:
	UPROPERTY(EditAnywhere , Category="Wander")
	float OffsetDistance = 150.f;

	UPROPERTY(EditAnywhere , Category="Wander")
	float WanderRadius = 100.f;

	UPROPERTY(EditAnywhere , Category="Wander")
	float MaxAngleChange = 30.f;
	
	UPROPERTY(EditAnywhere , Category="Wander")
	float AcceptanceRadius = 50.f;
	
	
private:
	float WanderAngle = 0.f;
};
