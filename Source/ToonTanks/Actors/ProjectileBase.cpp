// Mattia De Prisco 2020

#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("Projectile Mesh"));
    
    ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

    RootComponent = ProjectileMesh;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(
        TEXT("Projectile Movement"));

    ProjectileMovement->InitialSpeed = ProjectileMovement->MaxSpeed =
        MovementSpeed;

    InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
    Super::BeginPlay();
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

    if (!OtherActor || OtherActor == this || OtherActor == ProjectileOwner)
    {
        return;
    }

    UGameplayStatics::ApplyDamage(OtherActor, Damage,
                                  ProjectileOwner->GetInstigatorController(),
                                  this,
                                  DamageType);

    UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileHitEffect, GetActorLocation());

    Destroy();
}
