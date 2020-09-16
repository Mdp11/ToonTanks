// Copyrights Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "ToonTanks/Actors/ProjectileBase.h"

#include "ProjectileShock.generated.h"

class APawnBase;

UCLASS()
class TOONTANKS_API AProjectileShock : public AProjectileBase
{
    GENERATED_BODY()

private:

    UPROPERTY(EditAnywhere, meta=(AllowPrivateAcess="true"))
    UParticleSystemComponent* ShockPropagation{nullptr};

    UPROPERTY(EditAnywhere)
    float PropagationRate{0.5f};

    UPROPERTY()
    TArray<AActor*> AlreadyShockedEnemies;

    FTimerHandle ShockHandle;

    UPROPERTY(EditAnywhere)
    float ShockRadius{300.f};

    APawnBase* GetClosestPawn(AActor* ShockPropagatingActor) const;

public:
    AProjectileShock();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void PropagateShock(AActor* ShockPropagatingActor);
    void PropagateShockEffect(AActor* ShockedEnemy);
    void DematerializeProjectile();
    void PlayShockEffects(FVector& SparkLocation) const;

    virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                       UPrimitiveComponent* OtherComponent,
                       FVector NormalImpulse,
                       const FHitResult& HitResult) override;
};
