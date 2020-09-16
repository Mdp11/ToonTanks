// Copyrights Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "ToonTanks/Pawns/PawnBase.h"
#include "PawnTurret.generated.h"

class APawnTank;

UCLASS()
class TOONTANKS_API APawnTurret : public APawnBase
{
    GENERATED_BODY()

private:
    void CheckFireConditions();

public:
    APawnTurret()
    {
    }

    virtual void Tick(float DeltaTime) override;

    virtual void HandleDestruction() override;

protected:

    virtual void BeginPlay() override;

    void HandleFire();

    float GetDistanceFromPlayer() const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat", meta=(
        AllowPrivateAccess="true"))
    float FireRange{1000.f};

    FTimerHandle InitiateFireHandle;

    UPROPERTY()
    APawnTank* PlayerPawn{nullptr};
};
