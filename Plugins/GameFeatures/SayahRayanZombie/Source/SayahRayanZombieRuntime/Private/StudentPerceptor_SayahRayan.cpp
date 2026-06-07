// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptor_SayahRayan.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Items/Food.h"
#include "Items/Medkit.h"
#include "Items/Pistol.h"
#include "Items/Shotgun.h"
#include "PurgeZones/PurgeZone.h"
#include "Village/House/House.h"
#include "Zombies/BaseZombie.h"


UStudentPerceptor_SayahRayan::UStudentPerceptor_SayahRayan()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStudentPerceptor_SayahRayan::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptor_SayahRayan::OnPerceptionUpdated);
	}
}

void UStudentPerceptor_SayahRayan::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
	FString::Printf(TEXT("Saw Something!")));
	
	
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;

	
	AAIController* control = Cast<AAIController>(OwnerPawn->GetController());
	if (!control) return;
;
	
	UBlackboardComponent* board = control->GetBlackboardComponent();
	if (!board) return;

	
	//TODO add fullname into this

		
    if (Stimulus.WasSuccessfullySensed())
    {
        board->SetValueAsBool(FName("PerceptedSomething"), true);

        if (APurgeZone* SensedPurgeZone = Cast<APurgeZone>(Actor))
            board->SetValueAsObject(FName("PurgeZone"), SensedPurgeZone);

        if (ABaseZombie* SensedZombie = Cast<ABaseZombie>(Actor))
            board->SetValueAsObject(FName("Zombie"), SensedZombie);

        if (AHouse* SensedHouse = Cast<AHouse>(Actor))
            if (!board->GetValueAsObject(FName("House")))
                board->SetValueAsObject(FName("House"), SensedHouse);

        if (AFood* SensedFood = Cast<AFood>(Actor))
        {
            board->SetValueAsObject(FName("Food"), SensedFood);
            board->SetValueAsBool(FName("ItemSeen"), true);
        }
        else if (AMedkit* SensedMedkit = Cast<AMedkit>(Actor))
        {
            board->SetValueAsObject(FName("Medkit"), SensedMedkit);
            board->SetValueAsBool(FName("ItemSeen"), true);
        }
        else if (APistol* SensedPistol = Cast<APistol>(Actor))
        {
            board->SetValueAsObject(FName("Handgun"), SensedPistol);
            board->SetValueAsBool(FName("ItemSeen"), true);
        }
        else if (AShotgun* SensedShotgun = Cast<AShotgun>(Actor))
        {
            board->SetValueAsObject(FName("Shotgun"), SensedShotgun);
            board->SetValueAsBool(FName("ItemSeen"), true);
        }
    }
    else
    {
        // Clear only the specific thing that left perception


        if (Cast<AFood>(Actor))        board->SetValueAsObject(FName("Food"), nullptr);
        else if (Cast<AMedkit>(Actor)) board->SetValueAsObject(FName("Medkit"), nullptr);
        else if (Cast<APistol>(Actor)) board->SetValueAsObject(FName("Handgun"), nullptr);
        else if (Cast<AShotgun>(Actor))board->SetValueAsObject(FName("Shotgun"), nullptr);

        // Only clear ItemSeen if all item keys are gone
        if (!board->GetValueAsObject(FName("Food")) &&
            !board->GetValueAsObject(FName("Medkit")) &&
            !board->GetValueAsObject(FName("Handgun")) &&
            !board->GetValueAsObject(FName("Shotgun")))
        {
            board->SetValueAsBool(FName("ItemSeen"), false);
        }
    }

	if (Stimulus.WasSuccessfullySensed())
	{
		
		board->SetValueAsBool(FName("PerceptedSomething"), true);
		
		GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, FString::Printf(TEXT("Stimulus succeeded!")));
		if (APurgeZone* SensedPurgeZone = Cast<APurgeZone>(Actor) )
		{
			board->SetValueAsObject(FName("PurgeZone"), SensedPurgeZone);
		}
		
		if (ABaseZombie* SensedZombie = Cast<ABaseZombie>(Actor) )
		{
			board->SetValueAsObject(FName("Zombie"), SensedZombie);
		}

		if (AHouse* SensedHouse = Cast<AHouse>(Actor) )
		{
			if (!board->GetValueAsObject(FName("House")))
			{
				board->SetValueAsObject(FName("House"), SensedHouse);
			}
		}
		
		
	}
	
	
}
