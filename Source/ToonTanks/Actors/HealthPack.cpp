// Mattia De Prisco 2020


#include "HealthPack.h"

#include "ToonTanks/Pawns/PawnTank.h"

void AHealthPack::Empower()
{
    if(APawnTank* PlayerTank = Cast<APawnTank>(PlayerActor))
    {
        PlayerTank->Heal(HealValue);
    }
}

