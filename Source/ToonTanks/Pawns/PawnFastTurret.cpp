// Mattia De Prisco 2020

#include "PawnFastTurret.h"

#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Actors/ProjectileBase.h"

APawnFastTurret::APawnFastTurret()
{
    FireRate = 0.2f;
    FireRange = 1000.f;
    FireChargeDelay = 0.f;
}

void APawnFastTurret::BeginPlay()
{
    Super::BeginPlay();
}

void APawnFastTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APawnFastTurret::PreFire()
{
    if (bReadyToBurst)
    {
        if (FireChargeSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, FireChargeSound,
                                                  GetActorLocation());
        }

        GetWorld()->GetTimerManager().SetTimer(PreFireHandle, this,
                                               &APawnFastTurret::Fire,
                                               FireChargeSound->Duration);
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
    if(bReadyToFire)
    {
        bReadyToFire = false;
        Super::Fire();
        if (++ProjectileCount == ProjectileBurstLimit)
        {
            ProjectileCount = 0;
            bBursting = false;
            if (FireChargeSound)
            {
                GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle, this,
                                                       &APawnFastTurret::RestoreBurst,
                                                       BurstRate - FireChargeSound->Duration);
            }
            else
            {
                UE_LOG(LogTemp, Warning,
                       TEXT("Fire charge sound for fast turret not set."))
            }
        }
    }
}
