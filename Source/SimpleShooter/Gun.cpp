// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunRoot = CreateDefaultSubobject<USceneComponent>(TEXT("GunRoot"));
	SetRootComponent(GunRoot);

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(GunRoot);


}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(!OwnerPawn) {
		return nullptr;
	}
	return OwnerPawn->GetController();	
}

bool AGun::GunTrace(FHitResult& Hit, FVector& DirectionToGun) 
{
	AController* OwnerController = GetOwnerController();
	if(!OwnerController) {
		return false;
	}

	FVector TraceStart;
	FVector TraceEnd;
	FRotator TraceRotation;
	OwnerController->GetPlayerViewPoint(OUT TraceStart, OUT TraceRotation);
	TraceEnd = TraceStart + TraceRotation.Vector() * MaxRange;
	DirectionToGun = -TraceRotation.Vector();

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

void AGun::PullTrigger() 
{
	if(!GunMesh || !MuzzleFlash || !HitEffect) {
		return;
	}
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunMesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, GunMesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector DirectionToGun;
	bool Success = GunTrace(Hit, DirectionToGun);
	if(Success) 
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.Location, DirectionToGun.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);

		AActor* HitActor = Hit.GetActor();
		if(HitActor) {
			FPointDamageEvent DamageEvent(Damage, Hit, DirectionToGun, nullptr);
			HitActor->TakeDamage(Damage, DamageEvent, GetOwnerController(), this);
		} 
	}
	// DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
	// DrawDebugCamera(GetWorld(), Location, Rotation, 90, 2.0f, FColor::Red, true);
}

