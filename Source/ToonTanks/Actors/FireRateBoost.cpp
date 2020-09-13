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
    if (PlayerPawnTank)
    {
        PlayerPawnTank->BoostFireRate(1 / FireRateMultiplier, BoostDuration);
    }
    Destroy();
}

void AFireRateBoost::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
