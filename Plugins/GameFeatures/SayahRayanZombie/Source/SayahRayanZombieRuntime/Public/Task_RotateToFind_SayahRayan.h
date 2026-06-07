// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Survivor/SurvivorPawn.h"
#include "Task_RotateToFind_SayahRayan.generated.h"

/**
 * 
 */
UCLASS()
class SAYAHRAYANZOMBIERUNTIME_API UTask_RotateToFind_SayahRayan : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UTask_RotateToFind_SayahRayan();

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	ASurvivorPawn* GetSurvivor(const UBehaviorTreeComponent& OwnerComp);
	float RotationStepDegrees{5.f};
};
