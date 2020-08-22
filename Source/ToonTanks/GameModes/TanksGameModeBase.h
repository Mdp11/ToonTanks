// Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TanksGameModeBase.generated.h"

class APawnTurret;
class APawnTank;

UCLASS()
class TOONTANKS_API ATanksGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

private:
    APawnTank* PlayerTank;
    int32 TargetTurrets{0};

    int32 GetTargetTurretsCount() const;
    void HandleGameStart();
    void HandleGameOver(bool PlayerWon);
    
public:

    void ActorDied(AActor* DeadActor);

protected:

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintImplementableEvent)
    void GameStart();

    UFUNCTION(BlueprintImplementableEvent)
    void GameOver(bool PlayerWon);
};