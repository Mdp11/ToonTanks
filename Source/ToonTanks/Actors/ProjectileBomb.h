// Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "ToonTanks/Actors/ProjectileBase.h"
#include "ProjectileBomb.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AProjectileBomb : public AProjectileBase
{
    GENERATED_BODY()

public:
    AProjectileBomb();

protected:
    virtual void BeginPlay() override;

    virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                       UPrimitiveComponent* OtherComponent,
                       FVector NormalImpulse, const
                       FHitResult& HitResult) override;
};
