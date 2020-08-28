// Mattia De Prisco 2020

#include "PawnFastTurret.h"
#include "ToonTanks/Actors/ProjectileBase.h"

APawnFastTurret::APawnFastTurret()
{
    FireRate = 0.2f;
    FireRange = 2000.f;
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
    Super::Fire();
}
