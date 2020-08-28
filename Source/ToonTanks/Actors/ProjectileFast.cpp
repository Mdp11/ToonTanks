// Mattia De Prisco 2020

#include "ProjectileFast.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileFast::AProjectileFast() : AProjectileBase()
{
    ProjectileMovement->InitialSpeed = ProjectileMovement->MaxSpeed =
        MovementSpeed = 4400.f;
}

void AProjectileFast::BeginPlay()
{
    Super::BeginPlay();
}
