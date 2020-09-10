// Copyrights Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "ToonTanks/Actors/ProjectileBase.h"
#include "ProjectileFast.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AProjectileFast : public AProjectileBase
{
    GENERATED_BODY()

public:
    AProjectileFast();

protected:
    virtual void BeginPlay() override;
};
