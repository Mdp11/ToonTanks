// Copyrights Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "PawnFastTurret.generated.h"

UCLASS()
class TOONTANKS_API APawnFastTurret : public APawnTurret
{
    GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat", meta=(
        AllowPrivateAccess="true"))
    float BurstRate{3.0};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat", meta=(
        AllowPrivateAccess="true"))
    int ProjectileBurstLimit{5};

    UPROPERTY(EditAnywhere, Category="Sounds")
    USoundBase* ChargingInterruptSound{nullptr};

    int ProjectileCount{0};

    float BurstDelay{0.f};

    bool bReadyToBurst{true};

    bool bBursting{false};

    FTimerHandle BurstTimerHandle;

    void RestoreBurst() { bReadyToBurst = true; }

public:
    APawnFastTurret();

    virtual void Tick(float DeltaTime) override;

protected:

    virtual void BeginPlay() override;
    void InterruptFire();
    virtual void PreFire() override;
    virtual void Fire() override;
};
