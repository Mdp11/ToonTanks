// Copyrights Mattia De Prisco 2020

#include "FireRangeBoost.h"

#include "ToonTanks/Pawns/PawnTank.h"

AFireRangeBoost::AFireRangeBoost() : APowerUpBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AFireRangeBoost::Empower()
{
    if (!BoostedProjectileClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Boosted projectile class not set."))
        return;
    }

    APawnTank* PlayerPawnTank = Cast<APawnTank>(PlayerActor);
    if (!bPicked && PlayerPawnTank)
    {
        PlayerPawnTank->SetProjectileClass(BoostedProjectileClass);
        bPicked = true;
        SetActorHiddenInGame(true);
        GetWorld()->GetTimerManager().SetTimer(BoostTimer, this,
                                               &AFireRangeBoost::DeactivateFireRangeBoost,
                                               BoostDuration);
    }
}

void AFireRangeBoost::DeactivateFireRangeBoost()
{
    if (APawnTank* PlayerPawnTank = Cast<APawnTank>(PlayerActor))
    {
        PlayerPawnTank->RestoreDefaultProjectileClass();
        Destroy();
    }
}

void AFireRangeBoost::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
