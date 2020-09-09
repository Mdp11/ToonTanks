// Mattia De Prisco 2020

#include "HealthPack.h"

#include "ToonTanks/Pawns/PawnTank.h"

void AHealthPack::OnOverlap(UPrimitiveComponent* OverlappedComponent,
                            AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            int32 OtherBodyIndex, bool bFromSweep,
                            const FHitResult& SweepResult)
{
    if (OtherActor == PlayerActor)
    {
        Empower();
    }
}

void AHealthPack::Empower()
{
    if (APawnTank* PlayerTank = Cast<APawnTank>(PlayerActor))
    {
        if (PlayerTank->GetCurrentHealth() < PlayerTank->GetMaximumHealth())
        {
            PlayerTank->Heal(HealValue);
            Destroy();
        }
    }
}
