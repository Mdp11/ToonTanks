// Copyrights Mattia De Prisco 2020

#include "HealthPack.h"

#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Pawns/PawnTank.h"

void AHealthPack::Empower()
{
    if (APawnTank* PlayerPawnTank = Cast<APawnTank>(PlayerActor))
    {
        if (PlayerPawnTank->GetCurrentHealth() < PlayerPawnTank->
            GetMaximumHealth())
        {
            UGameplayStatics::PlaySoundAtLocation(this, PickUpSound,
                                                  GetActorLocation());
            PlayerPawnTank->Heal(HealValue);
            Destroy();
        }
    }
}

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
