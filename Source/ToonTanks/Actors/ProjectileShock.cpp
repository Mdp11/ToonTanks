// Copyrights Mattia De Prisco 2020

#include "ProjectileShock.h"

#include <stdbool.h>

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "ToonTanks/Pawns/PawnBase.h"

AProjectileShock::AProjectileShock() : AProjectileBase()
{
    ShockPropagation = CreateDefaultSubobject<UParticleSystemComponent>(
        "Shock propagation effect");
    ShockPropagation->SetAutoActivate(false);
    ProjectileMovement->InitialSpeed = ProjectileMovement->MaxSpeed =
        MovementSpeed = 2500.f;
    Damage = 20;
}

void AProjectileShock::BeginPlay()
{
    Super::BeginPlay();
}

void AProjectileShock::PropagateShock(AActor* OtherActor)
{
    ShockPropagation->Deactivate();
    TArray<FHitResult> ComponentsInExplosionRange;

    GetWorld()->SweepMultiByChannel(ComponentsInExplosionRange,
                                    OtherActor->GetActorLocation(),
                                    OtherActor->GetActorLocation(),
                                    FQuat::Identity, ECC_Pawn,
                                    FCollisionShape::MakeSphere(500.f));

    APawnBase* ClosestPawn = nullptr;

    ComponentsInExplosionRange.Sort([this](const auto& Lhs, const auto& Rhs)
    {
        return GetDistanceTo(Lhs.GetActor()) < GetDistanceTo(Rhs.GetActor());
    });

    for (const auto& Component : ComponentsInExplosionRange)
    {
        const auto PawnHit = Cast<APawnBase>(Component.GetActor());
        if (PawnHit && !AlreadyShockedPawns.Contains(PawnHit))
        {
            ClosestPawn = PawnHit;
            break;
        }
    }
    APawnBase* HitPawn = Cast<APawnBase>(OtherActor);
    if (HitPawn)
    {
        AlreadyShockedPawns.Add(HitPawn);
    }
    if (ClosestPawn)
    {
        OnHit(nullptr, ClosestPawn, nullptr, FVector{}, FHitResult{});
    }
    else
    {
        Destroy();
    }
}

void AProjectileShock::OnHit(UPrimitiveComponent* HitComponent,
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

    FVector SparkLocation{};

    UGameplayStatics::PlaySoundAtLocation(this, HitSound,
                                          GetActorLocation(),
                                          FRotator::ZeroRotator, 0.2f);

    APawnBase* OtherPawn = Cast<APawnBase>(OtherActor);
    if (OtherPawn)
    {
        SparkLocation = OtherActor->GetActorLocation();
        AlreadyShockedPawns.Add(OtherPawn);
        UGameplayStatics::ApplyDamage(OtherActor, Damage,
                                      ProjectileOwner->
                                      GetInstigatorController(),
                                      this,
                                      DamageType);

        const FTimerDelegate ShockDelegate = FTimerDelegate::CreateUObject(this,
            &AProjectileShock::PropagateShock,
            OtherActor);

        GetWorld()->GetTimerManager().SetTimer(ShockHandle,
                                               ShockDelegate,
                                               PropagationRate, false);

        ProjectileMesh->SetHiddenInGame(true);
        ProjectileTrailEffect->SetHiddenInGame(true);
        SetActorEnableCollision(false);

        FLatentActionInfo LatentInfo;
        LatentInfo.CallbackTarget = this;

        if (ShockPropagation)
        {
            ShockPropagation->Activate();
            auto ShockPropagationTargetLocation = OtherPawn->GetActorLocation();
            ShockPropagationTargetLocation.Z = 10.f;
            UKismetSystemLibrary::MoveComponentTo(ShockPropagation,
                                                  ShockPropagationTargetLocation,
                                                  FRotator::ZeroRotator, false,
                                                  false, 0.1f, false,
                                                  EMoveComponentAction::Move,
                                                  LatentInfo);
        }
    }
    else
    {
        SparkLocation = GetActorLocation();
        Destroy();
    }

    UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileHitEffect,
                                             SparkLocation,
                                             FRotator::ZeroRotator,
                                             {1.f, 1.f, 1.f});
}
