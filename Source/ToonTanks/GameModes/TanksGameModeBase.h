// Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TanksGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATanksGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

private:

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
