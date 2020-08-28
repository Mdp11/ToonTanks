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

    PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
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
    CheckFireConditions();
}

void APawnTurret::CheckFireConditions()
{
    if (!PlayerPawn)
    {
        return;
    }
    if (PlayerPawn->IsAlive() && GetDistanceFromPlayer() <= FireRange)
    {
        if (bReadyToFire)
        {
            Fire();
        }
    }
}

float APawnTurret::GetDistanceFromPlayer() const
{
    return PlayerPawn
               ? FVector::Dist(PlayerPawn->GetActorLocation(),
                               GetActorLocation())
               : 0.f;
}

void APawnTurret::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}
