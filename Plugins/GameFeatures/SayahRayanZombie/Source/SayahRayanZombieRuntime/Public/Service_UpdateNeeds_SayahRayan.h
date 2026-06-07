// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Service_UpdateNeeds_SayahRayan.generated.h"

/**
 * 
 */
UCLASS()
class SAYAHRAYANZOMBIERUNTIME_API UService_UpdateNeeds_SayahRayan : public UBTService
{
	GENERATED_BODY()
public:
	UService_UpdateNeeds_SayahRayan();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	float PreviousHealth       = 0.f;
	float TimeSinceLastHit     = 0.f;
	float AttackMemoryDuration = 2.f;
};
