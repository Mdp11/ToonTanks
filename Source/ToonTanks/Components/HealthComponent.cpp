// Copyrights Mattia De Prisco 2020

#include "HealthComponent.h"
#include "ToonTanks/GameModes/TanksGameModeBase.h"
#include "Kismet/GameplayStatics.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    Health = DefaultHealth;
    GameModeRef = Cast<ATanksGameModeBase>(
        UGameplayStatics::GetGameMode(GetWorld()));

    GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage,
                                  const UDamageType* DamageType,
                                  AController* InstigatedBy, AActor* DamagedBy)
{
    if (Damage == 0.f || Health <= 0.f)
    {
        return;
    }

    Health = FMath::Clamp(Health - Damage, 0.f, DefaultHealth);

    if (Health <= 0.f)
    {
        if (GameModeRef)
        {
            GameModeRef->ActorDied(GetOwner());
        }
        else
        {
            UE_LOG(LogTemp, Warning,
                   TEXT("HealthComponent: Pointer to GameMode not set"));
        }
    }
}

void UHealthComponent::Heal(const float HealValue)
{
    if (HealValue > 0.f)
    {
        Health = FMath::Clamp(Health + HealValue, 0.f, DefaultHealth);
    }
}
