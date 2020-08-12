// Mattia De Prisco 2020

#include "PawnTurret.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "Kismet/GameplayStatics.h"

APawnTurret::APawnTurret()
{
}

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
    Super::BeginPlay();
    GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this,
                                           &APawnTurret::CheckFireConditions,
                                           FireRate, true);
    PlayerPawn = Cast<APawnTank>(
        UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APawnTurret::CheckFireConditions()
{
    if (!PlayerPawn)
    {
        return;
    }
    if(GetDistanceFromPlayer() <= FireRange)
    {
        UE_LOG(LogTemp, Warning, TEXT("Fire!"));
    }
}

float APawnTurret::GetDistanceFromPlayer()
{
    UE_LOG(LogTemp, Warning, TEXT("Distance = %f"), FVector::Dist(PlayerPawn->GetActorLocation(),
                       GetActorLocation()));

    return PlayerPawn
               ? FVector::Dist(PlayerPawn->GetActorLocation(),
                               GetActorLocation())
               : 0.f;
}
