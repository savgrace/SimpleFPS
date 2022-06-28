// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    if(AIBehavior) {
        RunBehaviorTree(AIBehavior);

        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if(Blackboard) {
            Blackboard->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
            Blackboard->SetValueAsVector(TEXT("PlayerLocation"), GetPawn()->GetActorLocation());
            //UE_LOG(LogTemp, Warning, TEXT("Character Blackboard StartLocation set to %s"), *PlayerPawn->GetActorLocation().ToString());
        } else {
            UE_LOG(LogTemp, Warning, TEXT("Character %s Blackboard is null."), *GetName());
        }
    }
}

void AShooterAIController::Tick(float DeltaSeconds) 
{
    Super::Tick(DeltaSeconds);

    // AShooterCharacter* OwnerCharacter = Cast<AShooterCharacter>(GetCharacter());
	// if(OwnerCharacter && OwnerCharacter->IsDead()) {
    //     OwnerCharacter->DestroyConstructedComponents();
    //     this->Destroy();
	// 	return;
	// }
    
}

bool AShooterAIController::IsDead() const
{
    AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
    if(ControlledCharacter) {
        return ControlledCharacter->IsDead();
    }

    return true;
}
