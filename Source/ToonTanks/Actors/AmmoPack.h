// Copyrights Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"

#include "ProjectileBase.h"
#include "ToonTanks/Actors/PowerUpBase.h"
#include "AmmoPack.generated.h"

UCLASS()
class TOONTANKS_API AAmmoPack : public APowerUpBase
{
    GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Type", meta= (
        AllowPrivateAccess="true"))
    int WeaponType{1};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta= (AllowPrivateAccess="true")
    )
    int AmmoAmount{5};

public:
    AAmmoPack();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void Empower() override;
};
