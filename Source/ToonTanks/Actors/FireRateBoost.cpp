// Mattia De Prisco 2020

#include "FireRateBoost.h"

#include "ToonTanks/Pawns/PawnTank.h"

void AFireRateBoost::Empower()
{
    APawnTank* PlayerPawnTank = Cast<APawnTank>(PlayerActor);
    if (!bPicked && PlayerPawnTank)
    {
        // PlayerPawnTank->FireRate /= FireRateMultiplier;
        bPicked = true;
        SetActorHiddenInGame(true);
        GetWorld()->GetTimerManager().SetTimer(BoostTimer, this,
                                               &AFireRateBoost::DeactivateFireBoost,
                                               BoostDuration);
    }
}

void AFireRateBoost::DeactivateFireBoost()
{
    if (APawnTank* PlayerPawnTank = Cast<APawnTank>(PlayerActor))
    {
        // PlayerPawnTank->FireRate *= FireRateMultiplier;
        Destroy();
    }
}
