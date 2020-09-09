// Mattia De Prisco 2020

#include "PawnFastTurret.h"

#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

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
    UE_LOG(LogTemp, Warning, TEXT("Checking bReadyToBurst..."));
    if (bReadyToBurst)
    {
        UE_LOG(LogTemp, Warning, TEXT("bReadyToBurst = true"));
        if (FireChargeSound)
        {
            UE_LOG(LogTemp, Warning, TEXT("FireChargeSound pointer set"))
            FireChargeSound->Play();
        }

        GetWorld()->GetTimerManager().SetTimer(PreFireHandle, this,
                                               &APawnFastTurret::Fire,
                                               FireChargeDelay);
        bReadyToBurst = false;
    }
    else if (bBursting)
    {
        UE_LOG(LogTemp, Warning, TEXT("bBursting = true"))
        Fire();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("bReadyToBurst = bBursting = false"))
    }
}

void APawnFastTurret::Fire()
{
    bBursting = true;
    UE_LOG(LogTemp, Warning, TEXT("Checking ready to fire..."))
    if (bReadyToFire)
    {
        UE_LOG(LogTemp, Warning, TEXT("bReadyToFire = true"))
        bReadyToFire = false;
        Super::Fire();
        UE_LOG(LogTemp, Warning, TEXT("Checking burst reached..."))
        if (++ProjectileCount == ProjectileBurstLimit)
        {
            UE_LOG(LogTemp, Warning, TEXT("burst reached"))
            ProjectileCount = 0;
            bBursting = false;
            if (FireChargeSound)
            {
                GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle, this,
                    &APawnFastTurret::RestoreBurst,
                    BurstRate - FireChargeDelay);
            }
            else
            {
                UE_LOG(LogTemp, Warning,
                       TEXT("Fire charge sound for fast turret not set."))
            }
        }
    }
}
