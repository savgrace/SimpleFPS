// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay() 
{
    Super::BeginPlay();

    HUD = CreateWidget(this, HUDClass);
    if(HUD) {
        HUD->AddToViewport();
    }
}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner) 
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    HUD->RemoveFromViewport();

    UUserWidget* EndGameScreen = CreateWidget(this, bIsWinner ? WinScreenClass : LoseScreenClass);
    if(EndGameScreen) {
        EndGameScreen->AddToViewport();
    }

    FTimerManager& TimerManager = GetWorldTimerManager();
    TimerManager.SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

