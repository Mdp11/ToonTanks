// Copyrights Mattia De Prisco 2020

#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Pawns/PawnBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "Particles/ParticleSystemComponent.h"

AProjectileBase::AProjectileBase()
{
    PrimaryActorTick.bCanEverTick = false;

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("Projectile Mesh"));
    ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
    RootComponent = ProjectileMesh;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(
        TEXT("Projectile Movement"));
    ProjectileMovement->InitialSpeed = ProjectileMovement->MaxSpeed =
        MovementSpeed;

    ProjectileTrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(
        TEXT("Trail effect"));
    ProjectileTrailEffect->SetupAttachment(RootComponent);

    InitialLifeSpan = 10.f;
}

void AProjectileBase::BeginPlay()
{
    Super::BeginPlay();

    UGameplayStatics::PlaySoundAtLocation(this, LaunchSound,
                                          GetActorLocation());
}

void AProjectileBase::PlayHitEffects(FVector& HitParticleScale,
                                     USoundBase* SoundToPlay) const
{
    UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, GetActorLocation(),
                                          FRotator::ZeroRotator);

    UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileHitEffect,
                                             GetActorLocation(),
                                             FRotator::ZeroRotator,
                                             HitParticleScale);
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent,
                            AActor* OtherActor,
                            UPrimitiveComponent* OtherComponent,
                            FVector NormalImpulse, const FHitResult& HitResult)
{
    AActor* ProjectileOwner = GetOwner();

    if (ProjectileOwner && OtherActor && OtherActor != this)
    {
        FVector HitParticleScale{0.2f, 0.2f, 0.2f};
        APawnTank* PlayerTank = Cast<APawnTank>(OtherActor);
        USoundBase* SoundToPlay = HitSound;

        if (Cast<APawnBase>(OtherActor))
        {
            if (PlayerTank && PlayerTank->IsShieldActive())
            {
                SoundToPlay = ShieldHitSound;
            }
            else
            {
                HitParticleScale = {1.f, 1.f, 1.f};
                UGameplayStatics::ApplyDamage(OtherActor, Damage,
                                              ProjectileOwner->
                                              GetInstigatorController(), this,
                                              DamageType);
            }
        }

        PlayHitEffects(HitParticleScale, SoundToPlay);
    }

    Destroy();
}
