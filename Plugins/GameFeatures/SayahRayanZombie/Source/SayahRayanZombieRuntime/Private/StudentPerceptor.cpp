// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptor.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Items/Food.h"
#include "Items/Medkit.h"
#include "Items/Pistol.h"
#include "Items/Shotgun.h"
#include "PurgeZones/PurgeZone.h"
#include "Village/House/House.h"
#include "Zombies/BaseZombie.h"


UStudentPerceptor::UStudentPerceptor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStudentPerceptor::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptor::OnPerceptionUpdated);
	}
}

void UStudentPerceptor::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
	FString::Printf(TEXT("Saw Something!")));
	
	
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;
	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
FString::Printf(TEXT("PAwn went through!")));
	
	AAIController* control = Cast<AAIController>(OwnerPawn->GetController());
	if (!control) return;
	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Red, 
FString::Printf(TEXT("AI went through!")));
	
	UBlackboardComponent* board = control->GetBlackboardComponent();
	if (!board) return;
	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Blue, 
FString::Printf(TEXT("Board went through!")));
	
	


	if (Stimulus.WasSuccessfullySensed())
	{
		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, FString::Printf(TEXT("Stimulus succeeded!")));
		if (APurgeZone* SensedPurgeZone = Cast<APurgeZone>(Actor) )
		{
			board->SetValueAsObject(FName("PurgeZone"), SensedPurgeZone);
		}
		
		if (ABaseZombie* SensedZombie = Cast<ABaseZombie>(Actor) )
		{
			board->SetValueAsObject(FName("Zombie"), SensedZombie);
		}
		
		if (AFood* SensedFood = Cast<AFood>(Actor) )
		{
			board->SetValueAsObject(FName("Food"), SensedFood);
		}
		
		if (AMedkit* SensedMedkit = Cast<AMedkit>(Actor) )
		{
			board->SetValueAsObject(FName("Medkit"), SensedMedkit);
		}
		
		if (APistol* SensedPistol = Cast<APistol>(Actor) )
		{
			board->SetValueAsObject(FName("Handgun"), SensedPistol);
		}
		
		if (AShotgun* SensedShotgun = Cast<AShotgun>(Actor) )
		{
			board->SetValueAsObject(FName("Shotgun"), SensedShotgun);
		}
		
		if (AHouse* SensedHouse = Cast<AHouse>(Actor) )
		{
			board->SetValueAsObject(FName("House"), SensedHouse);
		}
		
		
	}
	
	
}
