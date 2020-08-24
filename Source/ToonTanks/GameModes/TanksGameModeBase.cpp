// Mattia De Prisco 2020

#include "TanksGameModeBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"

void ATanksGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}

void ATanksGameModeBase::ActorDied(AActor* DeadActor)
{
    if (DeadActor == PlayerTank)
    {
        PlayerTank->HandleDestruction();
        HandleGameOver(false);
        if (PlayerControllerRef)
        {
            PlayerControllerRef->SetPlayerEnabledState(false);
        }
    }
    else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
    {
        DestroyedTurret->HandleDestruction();
        if (--TargetTurrets == 0)
        {
            HandleGameOver(true);
        }
    }
}

int32 ATanksGameModeBase::GetTargetTurretsCount() const
{
    TArray<AActor*> TurretsArray;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(),
                                          APawnTurret::StaticClass(),
                                          TurretsArray);
    return TurretsArray.Num();
}

void ATanksGameModeBase::HandleGameStart()
{
    TargetTurrets = GetTargetTurretsCount();
    PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
    PlayerControllerRef = Cast<APlayerControllerBase>
        (UGameplayStatics::GetPlayerController(this, 0));

    GameStart();

    if (PlayerControllerRef)
    {
        PlayerControllerRef->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableHandle;
        const FTimerDelegate PlayerEnableDelegate =
            FTimerDelegate::CreateUObject(
                PlayerControllerRef,
                &APlayerControllerBase::SetPlayerEnabledState,
                true);

        GetWorld()->GetTimerManager().SetTimer(PlayerEnableHandle,
                                               PlayerEnableDelegate,
                                               StartDelay + 1,
                                               false);
    }
}

void ATanksGameModeBase::HandleGameOver(bool PlayerWon)
{
    GameOver(PlayerWon);
}
