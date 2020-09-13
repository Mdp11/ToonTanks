// Copyrights Mattia De Prisco 2020

#include "BulletBoost.h"

#include "ToonTanks/Pawns/PawnTank.h"

ABulletBoost::ABulletBoost() : APowerUpBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ABulletBoost::Empower()
{
    if (!BoostedProjectileClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Boosted projectile class not set."))
        return;
    }

    APawnTank* PlayerPawnTank = Cast<APawnTank>(PlayerActor);
    if (PlayerPawnTank)
    {
        PlayerPawnTank->BoostProjectile(BoostedProjectileClass, BoostDuration);
    }
    Destroy();
}

void ABulletBoost::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
