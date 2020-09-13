// Copyrights Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"

#include "ProjectileBase.h"
#include "ToonTanks/Actors/PowerUpBase.h"
#include "BulletBoost.generated.h"

UCLASS()
class TOONTANKS_API ABulletBoost : public APowerUpBase
{
    GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile Type", meta=
        (AllowPrivateAccess="true"))
    TSubclassOf<AProjectileBase> BoostedProjectileClass;

public:
    ABulletBoost();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void Empower() override;
};
