// Copyrights Mattia De Prisco 2020

#include "FireRateBoost.h"

#include "ToonTanks/Pawns/PawnTank.h"

AFireRateBoost::AFireRateBoost() : APowerUpBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AFireRateBoost::Empower()
{
    APawnTank* PlayerPawnTank = Cast<APawnTank>(PlayerActor);
    if (!bPicked && PlayerPawnTank)
    {
        PlayerPawnTank->AdjustFireRate(1 / FireRateMultiplier);
        bPicked = true;
        SetActorHiddenInGame(true);
        GetWorld()->GetTimerManager().SetTimer(BoostTimer, this,
                                               &AFireRateBoost::DeactivateFireRateBoost,
                                               BoostDuration);
    }
}

void AFireRateBoost::DeactivateFireRateBoost()
{
    if (APawnTank* PlayerPawnTank = Cast<APawnTank>(PlayerActor))
    {
        PlayerPawnTank->AdjustFireRate(FireRateMultiplier);
        Destroy();
    }
}

void AFireRateBoost::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
