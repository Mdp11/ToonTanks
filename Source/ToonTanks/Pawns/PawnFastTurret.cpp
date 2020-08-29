// Mattia De Prisco 2020

#include "PawnFastTurret.h"
#include "ToonTanks/Actors/ProjectileBase.h"

APawnFastTurret::APawnFastTurret()
{
    FireRate = 0.2f;
    FireRange = 1000.f;
}

void APawnFastTurret::BeginPlay()
{
    Super::BeginPlay();
}

void APawnFastTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APawnFastTurret::Fire()
{
    if(bReadyToBurst)
    {
        Super::Fire();
        if(++ProjectileCount == ProjectileBurstLimit)
        {
            ProjectileCount = 0;
            bReadyToBurst = false;
            GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle, this, 
            &APawnFastTurret::RestoreBurst, BurstRate);
        }
    }
}
