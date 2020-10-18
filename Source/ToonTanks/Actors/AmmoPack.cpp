// Copyrights Mattia De Prisco 2020

#include "AmmoPack.h"

#include "ToonTanks/Pawns/PawnTank.h"

AAmmoPack::AAmmoPack() : APowerUpBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AAmmoPack::Empower()
{
    APawnTank* PlayerPawnTank = Cast<APawnTank>(PlayerActor);
    if (PlayerPawnTank)
    {
        PlayerPawnTank->AddAmmo(WeaponType, AmmoAmount);
    }
    Destroy();
}

void AAmmoPack::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
