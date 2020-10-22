// Copyrights Mattia De Prisco 2020

#include "ProjectileBomb.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Pawns/PawnBase.h"

AProjectileBomb::AProjectileBomb() : AProjectileBase()
{
}

void AProjectileBomb::BeginPlay()
{
    Super::BeginPlay();

}
void AProjectileBomb::GetPawnsInExplosionRange(
    TSet<APawnBase*>& OutPawnsInExplosionRange,
    const FVector ExplosionCenter) const
{
    TArray<FHitResult> ComponentsInExplosionRange;
    GetWorld()->SweepMultiByChannel(ComponentsInExplosionRange, ExplosionCenter,
                                    ExplosionCenter, FQuat::Identity, ECC_Pawn,
                                    FCollisionShape::MakeSphere(BombRadius));

    for (const auto& HitComponent : ComponentsInExplosionRange)
    {
        const auto ActorHit = Cast<APawnBase>(HitComponent.GetActor());
        if (ActorHit)
        {
            OutPawnsInExplosionRange.Add(ActorHit, nullptr);
        }
    }
}

void AProjectileBomb::PlayExplosionEffects() const
{
    UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileHitEffect,
                                             GetActorLocation(),
                                             FRotator::ZeroRotator, {
                                                 6.5f, 6.5f, 6.5f
                                             });

    UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation(),
                                          FRotator::ZeroRotator, 0.2f);
}

void AProjectileBomb::OnHit(UPrimitiveComponent* HitComponent,
                            AActor* OtherActor,
                            UPrimitiveComponent* OtherComponent,
                            FVector NormalImpulse, const FHitResult& HitResult)
{
    AActor* ProjectileOwner = GetOwner();

    if (ProjectileOwner && OtherActor && OtherActor != this)
    {
        TSet<APawnBase*> HitPawns;
        GetPawnsInExplosionRange(HitPawns,
                                 HitComponent->GetComponentLocation());

        for (const auto& Pawn : HitPawns)
        {
            UGameplayStatics::ApplyDamage(Pawn, Damage,
                                          ProjectileOwner->
                                          GetInstigatorController(), this,
                                          DamageType);
        }

        PlayExplosionEffects();
    }

    Destroy();
}
