// Copyrights Mattia De Prisco 2020

#include "ProjectileShock.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "ToonTanks/Pawns/PawnBase.h"

AProjectileShock::AProjectileShock() : AProjectileBase()
{
    ShockPropagation = CreateDefaultSubobject<UParticleSystemComponent>(
        "Shock propagation effect");
    ShockPropagation->SetAutoActivate(false);
}

void AProjectileShock::BeginPlay()
{
    Super::BeginPlay();
}

APawnBase* AProjectileShock::GetClosestPawn(AActor* ShockPropagatingActor) const
{
    TArray<FHitResult> ComponentsInShockRange;

    GetWorld()->SweepMultiByChannel(ComponentsInShockRange,
                                    ShockPropagatingActor->GetActorLocation(),
                                    ShockPropagatingActor->GetActorLocation(),
                                    FQuat::Identity, ECC_Pawn,
                                    FCollisionShape::MakeSphere(ShockRadius));

    ComponentsInShockRange.Sort(
        [&ShockPropagatingActor](const auto& Lhs, const auto& Rhs)
        {
            return FVector::Dist(ShockPropagatingActor->GetActorLocation(),
                                 Lhs.GetActor()->GetActorLocation()) <
                FVector::Dist(ShockPropagatingActor->GetActorLocation(),
                              Rhs.GetActor()->GetActorLocation());
        });

    for (const auto& HitResult : ComponentsInShockRange)
    {
        const auto PawnHit = Cast<APawnBase>(HitResult.GetActor());
        if (PawnHit && !AlreadyShockedEnemies.Contains(PawnHit))
        {
            return PawnHit;
        }
    }

    return nullptr;
}

void AProjectileShock::PropagateShock(AActor* ShockPropagatingActor)
{
    ShockPropagation->Deactivate();

    APawnBase* ClosestPawn = GetClosestPawn(ShockPropagatingActor);

    if (ClosestPawn)
    {
        OnHit(nullptr, ClosestPawn, nullptr, FVector{}, FHitResult{});
    }
    else
    {
        Destroy();
    }
}

void AProjectileShock::PropagateShockEffect(AActor* ShockedEnemy)
{
    if (ShockPropagation)
    {
        FLatentActionInfo LatentInfo;
        LatentInfo.CallbackTarget = this;

        ShockPropagation->Activate();

        auto ShockPropagationTargetLocation = ShockedEnemy->GetActorLocation();
        ShockPropagationTargetLocation.Z = 10.f;
        UKismetSystemLibrary::MoveComponentTo(ShockPropagation,
                                              ShockPropagationTargetLocation,
                                              FRotator::ZeroRotator, false,
                                              false, 0.1f, false,
                                              EMoveComponentAction::Move,
                                              LatentInfo);
    }
}

void AProjectileShock::DematerializeProjectile()
{
    ProjectileMesh->SetHiddenInGame(true);
    ProjectileTrailEffect->SetHiddenInGame(true);
    SetActorEnableCollision(false);
}

void AProjectileShock::PlayShockEffects(FVector& SparkLocation) const
{
    UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation(),
                                          FRotator::ZeroRotator, 0.2f);

    UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileHitEffect,
                                             SparkLocation,
                                             FRotator::ZeroRotator, {
                                                 1.f, 1.f, 1.f
                                             });
}

void AProjectileShock::OnHit(UPrimitiveComponent* HitComponent,
                             AActor* OtherActor,
                             UPrimitiveComponent* OtherComponent,
                             FVector NormalImpulse, const FHitResult& HitResult)
{
    AActor* ProjectileOwner = GetOwner();

    if (ProjectileOwner && OtherActor && OtherActor != this)
    {
        FVector SparkLocation;

        if (Cast<APawnBase>(OtherActor))
        {
            SparkLocation = OtherActor->GetActorLocation();
            UGameplayStatics::ApplyDamage(OtherActor, Damage,
                                          ProjectileOwner->
                                          GetInstigatorController(), this,
                                          DamageType);

            AlreadyShockedEnemies.Add(OtherActor);

            const FTimerDelegate ShockDelegate = FTimerDelegate::CreateUObject(
                this, &AProjectileShock::PropagateShock, OtherActor);

            GetWorld()->GetTimerManager().SetTimer(ShockHandle, ShockDelegate,
                                                   PropagationRate, false);

            DematerializeProjectile();
            PropagateShockEffect(OtherActor);
        }
        else
        {
            SparkLocation = GetActorLocation();
            Destroy();
        }

        PlayShockEffects(SparkLocation);
    }
}
