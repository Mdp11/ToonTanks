// Copyrights Mattia De Prisco 2020

#include "PawnTurret.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "Kismet/GameplayStatics.h"

void APawnTurret::BeginPlay()
{
    Super::BeginPlay();

    PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorld()->GetTimerManager().SetTimer(InitiateFireHandle, this,
                                           &APawnTurret::CheckFireConditions,
                                           FireRate, true);
}

void APawnTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    HandleFire();
}

bool APawnTurret::IsPlayerDirectlyInSight() const
{
    FHitResult ObjectInSightHitResult;
    GetWorld()->LineTraceSingleByChannel(ObjectInSightHitResult,
                                         ProjectileSpawnPoint->
                                         GetComponentLocation(),
                                         PlayerPawn->GetActorLocation(),
                                         ECC_Visibility);

    const auto ActorInSight = ObjectInSightHitResult.GetActor();

    return ActorInSight && Cast<APawnTank>(ActorInSight);
}

void APawnTurret::HandleFire()
{
    const bool FirePaused = GetWorld()->GetTimerManager().IsTimerPaused(
        InitiateFireHandle);

    if (PlayerPawn && GetDistanceFromPlayer() <= FireRange)
    {
        RotateTurret(PlayerPawn->GetActorLocation());
        if (IsPlayerDirectlyInSight())
        {
            if (FirePaused)
            {
                GetWorld()->GetTimerManager().UnPauseTimer(InitiateFireHandle);
            }
        }
    }
    else if (!FirePaused)
    {
        GetWorld()->GetTimerManager().PauseTimer(InitiateFireHandle);
    }
}

void APawnTurret::CheckFireConditions()
{
    if (PlayerPawn && PlayerPawn->IsAlive() && GetDistanceFromPlayer() <=
        FireRange && bReadyToFire)
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
