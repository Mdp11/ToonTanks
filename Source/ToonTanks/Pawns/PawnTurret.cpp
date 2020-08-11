// Mattia De Prisco 2020

#include "PawnTurret.h"

APawnTurret::APawnTurret()
{
}

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
    Super::BeginPlay();
    GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this,
                                           &APawnTurret::CheckFireConditions,
                                           FireRate, true);
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APawnTurret::CheckFireConditions()
{
    UE_LOG(LogTemp, Warning, TEXT("Fire!"));
}
