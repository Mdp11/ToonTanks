// Copyrights Mattia De Prisco 2020

#include "HealthComponent.h"
#include "ToonTanks/GameModes/TanksGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    // ...
}

// Called when the game starts
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
    if(HealValue > 0.f)
    {
        Health = FMath::Clamp(Health + HealValue, 0.f, DefaultHealth);
    }
}
