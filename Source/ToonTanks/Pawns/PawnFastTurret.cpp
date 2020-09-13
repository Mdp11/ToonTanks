// Copyrights Mattia De Prisco 2020

#include "PawnFastTurret.h"

#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

APawnFastTurret::APawnFastTurret()
{
    FireRate = 0.2f;
    FireRange = 1000.f;
}

void APawnFastTurret::BeginPlay()
{
    Super::BeginPlay();
    FireChargeDelay = 0.0001f;
    if (FireChargeSound && FireChargeSound->Sound)
    {
        BurstDelay = FireChargeSound->Sound->Duration;
    }
}

void APawnFastTurret::InterruptFire()
{
    ProjectileCount = 0;
    bReadyToBurst = true;
    bBursting = false;

    if(GetWorld()->GetTimerManager().IsTimerActive(PreFireHandle))
    {
        GetWorld()->GetTimerManager().ClearTimer(PreFireHandle);
    }
}

void APawnFastTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (PlayerPawn && GetDistanceFromPlayer() > FireRange)
    {
        InterruptFire();
    }
}

void APawnFastTurret::PreFire()
{
    if (bReadyToBurst)
    {
        if (FireChargeSound)
        {
            FireChargeSound->Play();
        }

        GetWorld()->GetTimerManager().SetTimer(PreFireHandle, this,
                                               &APawnFastTurret::Fire,
                                               BurstDelay);
        bReadyToBurst = false;
    }
    else if (bBursting)
    {
        Fire();
    }
}

void APawnFastTurret::Fire()
{
    bBursting = true;
    if (bReadyToFire)
    {
        bReadyToFire = false;
        Super::Fire();
        if (++ProjectileCount == ProjectileBurstLimit)
        {
            ProjectileCount = 0;
            bBursting = false;

            GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle, this,
                                                   &APawnFastTurret::RestoreBurst,
                                                   BurstRate - BurstDelay);
        }
    }
}
