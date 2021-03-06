// Copyrights Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "ToonTanks/Actors/PowerUpBase.h"
#include "HealthPack.generated.h"

UCLASS()
class TOONTANKS_API AHealthPack : public APowerUpBase
{
    GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere)
    float HealValue{20.f};

protected:
    virtual void Empower() override;

    virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent,
                           AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep,
                           const FHitResult& SweepResult) override;
};
