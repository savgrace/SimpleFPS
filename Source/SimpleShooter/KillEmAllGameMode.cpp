// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "ShooterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled) 
{
    Super::PawnKilled(PawnKilled);

    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if(PlayerController) {
        EndGame(false);
    }

    for(AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld())) {
        if(!Controller->IsDead()) {
            return;
        }
    }

    EndGame(true);
    UE_LOG(LogTemp, Warning, TEXT("A Pawn was killed."));
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner) 
{
    auto Range = TActorRange<AController>(GetWorld());
    for(AController* Controller : Range) 
    {
        // Everyone else wins if player loses
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
}
