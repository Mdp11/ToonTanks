// Copyrights Mattia De Prisco 2020

#include "PawnTurret.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "Kismet/GameplayStatics.h"

void APawnTurret::BeginPlay()
{
    Super::BeginPlay();

    PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void APawnTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    HandleFire();
}

void APawnTurret::HandleFire()
{
    const bool IsLoadingFire = GetWorld()->GetTimerManager().IsTimerActive(
        InitiateFireHandle);

    if (PlayerPawn && GetDistanceFromPlayer() <= FireRange)
    {
        RotateTurret(PlayerPawn->GetActorLocation());
        if (!IsLoadingFire)
        {
            GetWorld()->GetTimerManager().SetTimer(InitiateFireHandle, this,
                                                   &APawnTurret::CheckFireConditions,
                                                   FireRate);
        }
    }
    else if (IsLoadingFire)
    {
        GetWorld()->GetTimerManager().ClearTimer(InitiateFireHandle);
    }
}

void APawnTurret::CheckFireConditions()
{
    if (PlayerPawn && PlayerPawn->IsAlive() &&
        GetDistanceFromPlayer() <= FireRange && bReadyToFire)
    {
        PreFire();
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
