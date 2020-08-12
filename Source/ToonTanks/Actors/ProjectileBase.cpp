// Mattia De Prisco 2020

#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("Projectile Mesh"));
    RootComponent = ProjectileMesh;
    
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(
        TEXT("Projectile Movement"));

    ProjectileMovement->InitialSpeed = ProjectileMovement->MaxSpeed = MovementSpeed;

    InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
    Super::BeginPlay();
}
