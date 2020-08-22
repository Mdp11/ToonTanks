// Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "HealthComponent.generated.h"

class ATanksGameModeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere)
    float DefaultHealth = 100.f;
    float Health = 0.f;

    UPROPERTY()
    ATanksGameModeBase* GameModeRef{nullptr};

public:
    // Sets default values for this component's properties
    UHealthComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    UFUNCTION()
    void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType*
                    DamageType, AController* InstigatedBy, AActor* DamagedBy);
};
