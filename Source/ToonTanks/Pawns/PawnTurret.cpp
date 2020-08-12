// Mattia De Prisco 2020

#include "PawnTurret.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "Kismet/GameplayStatics.h"

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
    PlayerPawn = Cast<APawnTank>(
        UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!PlayerPawn || GetDistanceFromPlayer() > FireRange)
    {
        return;
    }
    RotateTurret(PlayerPawn->GetActorLocation());
}

void APawnTurret::CheckFireConditions()
{
    if (!PlayerPawn)
    {
        return;
    }
    if (GetDistanceFromPlayer() <= FireRange)
    {
        Fire();
    }
}

float APawnTurret::GetDistanceFromPlayer() const
{
    return PlayerPawn
               ? FVector::Dist(PlayerPawn->GetActorLocation(),
                               GetActorLocation())
               : 0.f;
}
