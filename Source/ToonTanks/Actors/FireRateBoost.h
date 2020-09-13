// Copyrights Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "ToonTanks/Actors/PowerUpBase.h"
#include "FireRateBoost.generated.h"

UCLASS()
class TOONTANKS_API AFireRateBoost : public APowerUpBase
{
    GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere)
    float FireRateMultiplier{3.f};

public:
    AFireRateBoost();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void Empower() override;
};
