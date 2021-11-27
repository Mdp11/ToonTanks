// Copyrights Mattia De Prisco 2020

#include "PawnBase.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanks/Actors/ProjectileBase.h"
#include "ToonTanks/Components/HealthComponent.h"
#include "Components/AudioComponent.h"

APawnBase::APawnBase()
{
    PrimaryActorTick.bCanEverTick = true;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(
        TEXT("Capsule Collider"));
    RootComponent = CapsuleComponent;

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base mesh"));
    BaseMesh->SetupAttachment(RootComponent);

    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("Turret mesh"));
    TurretMesh->SetupAttachment(BaseMesh);

    ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(
        TEXT("Projectile spawn point"));
    ProjectileSpawnPoint->SetupAttachment(TurretMesh);

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(
        TEXT("Health component"));

    FireChargeSound = CreateDefaultSubobject<UAudioComponent>(
        TEXT("Fire charge audio component"));
    FireChargeSound->bAutoActivate = false;
}

void APawnBase::BeginPlay()
{
    Super::BeginPlay();
    if (FireChargeSound && FireChargeSound->Sound)
    {
        FireChargeDelay = FireChargeSound->Sound->GetDuration();
    }
}

void APawnBase::RotateTurret(const FVector TargetLocation)
{
    const FVector StartLocation{TurretMesh->GetComponentLocation()};

    const FVector TargetLocationClean{
        TargetLocation.X, TargetLocation.Y, StartLocation.Z
    };

    const FRotator TurretRotation{
        (TargetLocationClean - StartLocation).Rotation()
    };

    TurretMesh->SetWorldRotation(TurretRotation);
}

void APawnBase::PreFire()
{
    if (FireChargeSound)
    {
        FireChargeSound->Play();
    }

    GetWorld()->GetTimerManager().SetTimer(PreFireHandle, this,
                                           &APawnBase::Fire, FireChargeDelay);
    bReadyToFire = false;
}

void APawnBase::Fire()
{
    if (!ProjectileClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Projectile class not set."))
        return;
    }

    AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(
        ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(),
        ProjectileSpawnPoint->GetComponentRotation());
    TempProjectile->SetOwner(this);

    GetWorld()->GetTimerManager().SetTimer(FireRateHandle, this,
                                           &APawnBase::RestoreFireAbility,
                                           FireRate - FireChargeDelay);
}

void APawnBase::HandleDestruction()
{
    UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles,
                                             GetActorLocation());
    UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());

    GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathShake);
}

float APawnBase::GetCurrentHealth() const
{
    return HealthComponent->GetCurrentHealth();
}

float APawnBase::GetMaximumHealth() const
{
    return HealthComponent->GetDefaultHealth();
}

void APawnBase::StopChargingSound() const
{
    if (FireChargeSound && FireChargeSound->IsPlaying())
    {
        FireChargeSound->Stop();
    }
}
