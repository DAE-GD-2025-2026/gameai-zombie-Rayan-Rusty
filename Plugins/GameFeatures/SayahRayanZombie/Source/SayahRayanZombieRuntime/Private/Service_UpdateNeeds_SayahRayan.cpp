// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_UpdateNeeds_SayahRayan.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Survivor/SurvivorPawn.h"

UService_UpdateNeeds_SayahRayan::UService_UpdateNeeds_SayahRayan()
{
	NodeName = "Refresh Boolean";
	Interval = 0.1f;
}

void UService_UpdateNeeds_SayahRayan::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return;

	ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(Controller->GetPawn());
	if (!Survivor) return;

	UBlackboardComponent* Board = OwnerComp.GetBlackboardComponent();
	if (!Board) return;

	UHealthComponent* HealthComp = Survivor->FindComponentByClass<UHealthComponent>();
	UStaminaComponent* StaminaComp = Survivor->FindComponentByClass<UStaminaComponent>();
	if (!HealthComp || !StaminaComp) return;

	float MissingHealth = HealthComp->GetMaxHealth() - HealthComp->GetHealth();
	float MissingStamina =  StaminaComp->GetMaxStamina() - StaminaComp->GetCurrentStamina();
	
	Board->SetValueAsBool(FName("NeedsHealthOrStamina"), MissingHealth > 0.f || MissingStamina > 0.f);
	
}
