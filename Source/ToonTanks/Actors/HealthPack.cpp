// Mattia De Prisco 2020

#include "HealthPack.h"


#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Pawns/PawnTank.h"

void AHealthPack::Empower()
{
    if (APawnTank* PlayerPawnTank = Cast<APawnTank>(PlayerActor))
    {
        if (PlayerPawnTank->GetCurrentHealth() < PlayerPawnTank->GetMaximumHealth())
        {
            PlayerPawnTank->Heal(HealValue);
            Destroy();
        }
    }
}
