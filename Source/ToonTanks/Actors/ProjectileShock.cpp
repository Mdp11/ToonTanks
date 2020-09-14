// Copyrights Mattia De Prisco 2020

#include "ProjectileShock.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ToonTanks/Pawns/PawnBase.h"

AProjectileShock::AProjectileShock() : AProjectileBase()
{
    ProjectileMovement->InitialSpeed = ProjectileMovement->MaxSpeed =
        MovementSpeed = 12000.f;
    Damage = 20;
}

void AProjectileShock::BeginPlay()
{
    Super::BeginPlay();
}

void AProjectileShock::PropagateShock(AActor* OtherActor)
{
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
        const FRotator SparkDirection =
            UKismetMathLibrary::GetDirectionUnitVector(
                GetActorLocation(), ClosestPawn->GetActorLocation()).Rotation();

        UGameplayStatics::SpawnEmitterAtLocation(this,
                                                 ShockPropagationEffect,
                                                 GetActorLocation(),
                                                 SparkDirection);
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

    UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileHitEffect,
                                             GetActorLocation(),
                                             FRotator::ZeroRotator,
                                             {1.f, 1.f, 1.f});

    UGameplayStatics::PlaySoundAtLocation(this, HitSound,
                                          GetActorLocation(),
                                          FRotator::ZeroRotator, 0.2f);

    if (Cast<APawnBase>(OtherActor))
    {
        UGameplayStatics::ApplyDamage(OtherActor, Damage,
                                      ProjectileOwner->
                                      GetInstigatorController(),
                                      this,
                                      DamageType);

        ShockDelegate = FTimerDelegate::CreateUObject(this,
            &AProjectileShock::PropagateShock,
            OtherActor);

        GetWorld()->GetTimerManager().SetTimer(ShockHandle,
                                               ShockDelegate,
                                               1.f, false);

        SetActorHiddenInGame(true);
    }
    else
    {
        Destroy();
    }
}
