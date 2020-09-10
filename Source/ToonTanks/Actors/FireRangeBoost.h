// Copyrights Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"

#include "ProjectileBase.h"
#include "ToonTanks/Actors/PowerUpBase.h"
#include "FireRangeBoost.generated.h"

UCLASS()
class TOONTANKS_API AFireRangeBoost : public APowerUpBase
{
    GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile Type", meta=
        (AllowPrivateAccess="true"))
    TSubclassOf<AProjectileBase> BoostedProjectileClass;

    bool bPicked{false};

    void DeactivateFireRangeBoost();

public:
    AFireRangeBoost();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void Empower() override;
};
