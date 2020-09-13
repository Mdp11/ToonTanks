// Copyrights Mattia De Prisco 2020

#include "ProjectileBomb.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Pawns/PawnBase.h"

AProjectileBomb::AProjectileBomb() : AProjectileBase()
{
    ProjectileMovement->InitialSpeed = ProjectileMovement->MaxSpeed =
        MovementSpeed = 2200.f;
}

void AProjectileBomb::BeginPlay()
{
    Super::BeginPlay();
}

void AProjectileBomb::OnHit(UPrimitiveComponent* HitComponent,
                            AActor* OtherActor,
                            UPrimitiveComponent* OtherComponent,
                            FVector NormalImpulse, const FHitResult& HitResult)
{
    AActor* ProjectileOwner = GetOwner();

    if (!ProjectileOwner)
    {
        return;
    }

    if (!OtherActor || OtherActor == this)
    {
        return;
    }
    TArray<FHitResult> ComponentsInExplosionRange;
    GetWorld()->SweepMultiByChannel(ComponentsInExplosionRange,
                                    HitComponent->GetComponentLocation(),
                                    HitComponent->GetComponentLocation(),
                                    FQuat::Identity, ECC_Pawn,
                                    FCollisionShape::MakeSphere(300.f));

    UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileHitEffect,
                                             GetActorLocation(),
                                             FRotator::ZeroRotator, {
                                                 6.5f, 6.5f, 6.5f
                                             });
											 
	UGameplayStatics::PlaySoundAtLocation(this, HitSound,
	GetActorLocation(),
	  FRotator::ZeroRotator, 0.2f);

    TSet<APawnBase*> HitPawns;
    for (const auto& Component : ComponentsInExplosionRange)
    {
        const auto ActorHit = Cast<APawnBase>(Component.GetActor());
        if (ActorHit)
        {
            HitPawns.Add(ActorHit, nullptr);
        }
    }

    for (const auto& Pawn : HitPawns)
    {
        UGameplayStatics::ApplyDamage(Pawn, Damage,
                                      ProjectileOwner->
                                      GetInstigatorController(),
                                      this,
                                      DamageType);
    }
    Destroy();
}
