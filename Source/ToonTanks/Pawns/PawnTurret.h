// Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "ToonTanks/Pawns/PawnBase.h"
#include "PawnTurret.generated.h"

class APawnTank;

/**
 * 
 */
UCLASS()
class TOONTANKS_API APawnTurret : public APawnBase
{
    GENERATED_BODY()

private:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat", meta=(
        AllowPrivateAccess="true"))
    float FireRate{2.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat", meta=(
        AllowPrivateAccess="true"))
    float FireRange{1000.f};

    FTimerHandle FireRateTimerHandle;

    UPROPERTY()
    APawnTank* PlayerPawn{nullptr};

    void CheckFireConditions();
    float GetDistanceFromPlayer() const;

public:
    APawnTurret();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    virtual void HandleDestruction() override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};
