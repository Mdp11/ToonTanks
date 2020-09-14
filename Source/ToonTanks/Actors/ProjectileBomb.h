// Copyrights Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "ToonTanks/Actors/ProjectileBase.h"
#include "ProjectileBomb.generated.h"

class APawnBase;

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
                       FVector NormalImpulse,
                       const FHitResult& HitResult) override;
private:
    UPROPERTY(EditAnywhere)
    float BombRadius{300.f};

    void GetPawnsInExplosionRange(TSet<APawnBase*>& OutPawnsInExplosionRange,
                                  const FVector ExplosionCenter) const;
    void PlayExplosionEffects() const;
};
