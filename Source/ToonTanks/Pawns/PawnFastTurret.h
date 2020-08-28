// Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "PawnFastTurret.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API APawnFastTurret : public APawnTurret
{
    GENERATED_BODY()

private:

    int ProjectileBurstLimit{5};
    int ProjectileCount{0};

public:
    APawnFastTurret();
    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void Fire() override;
};
