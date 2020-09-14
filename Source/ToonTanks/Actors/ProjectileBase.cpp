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

    ProjectileTrailEffect = CreateDefaultSubobject<UParticleSystemComponent>
        (TEXT("Trail effect"));
    ProjectileTrailEffect->SetupAttachment(RootComponent);

    InitialLifeSpan = 10.f;
}

void AProjectileBase::BeginPlay()
{
    Super::BeginPlay();

    UGameplayStatics::PlaySoundAtLocation(this, LaunchSound,
                                          GetActorLocation());
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent,
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

    FVector ParticleScale;
    APawnTank* PlayerTank = Cast<APawnTank>(OtherActor);
    if (!Cast<APawnBase>(OtherActor))
    {
        ParticleScale = {0.2f, 0.2f, 0.2f};
        UGameplayStatics::PlaySoundAtLocation(this, HitSound,
                                              GetActorLocation(),
                                              FRotator::ZeroRotator, 0.2f);
    }
    else if (PlayerTank && PlayerTank->IsShieldActive())
    {
        ParticleScale = {0.2f, 0.2f, 0.2f};
        UGameplayStatics::PlaySoundAtLocation(this, ShieldHitSound,
                                              GetActorLocation());
    }
    else
    {
        ParticleScale = {1.f, 1.f, 1.f};
        UGameplayStatics::ApplyDamage(OtherActor, Damage,
                                      ProjectileOwner->
                                      GetInstigatorController(),
                                      this,
                                      DamageType);
        UGameplayStatics::PlaySoundAtLocation(this, HitSound,
                                              GetActorLocation(),
                                              FRotator::ZeroRotator, 0.2f);
    }

    UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileHitEffect,
                                             GetActorLocation(),
                                             FRotator::ZeroRotator,
                                             ParticleScale);

    Destroy();
}
