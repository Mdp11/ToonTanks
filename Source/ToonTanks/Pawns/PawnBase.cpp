// Mattia De Prisco 2020

#include "PawnBase.h"
#include "Components/CapsuleComponent.h"
#include "ToonTanks/Actors/ProjectileBase.h"

// Sets default values
APawnBase::APawnBase()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
}

void APawnBase::RotateTurret(FVector TargetLocation)
{
    const FVector StartLocation{TurretMesh->GetComponentLocation()};

    const FVector TargetLocationClean{
        TargetLocation.X, TargetLocation.Y,
        StartLocation.Z
    };

    const FRotator TurretRotation{
        (TargetLocationClean - StartLocation).Rotation()
    };

    TurretMesh->SetWorldRotation(TurretRotation);
}

void APawnBase::Fire()
{
    if (ProjectileClass)
    {
        AProjectileBase* TempProjectile = GetWorld()
            ->SpawnActor<AProjectileBase>(ProjectileClass, ProjectileSpawnPoint
                                          ->GetComponentLocation(),
                                          ProjectileSpawnPoint->
                                          GetComponentRotation());
        TempProjectile->SetOwner(this);
    }
}

void APawnBase::HandleDestruction()
{
}
