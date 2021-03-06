// Copyrights Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "PawnBase.generated.h"

class UCapsuleComponent;
class AProjectileBase;
class UHealthComponent;

UCLASS()
class TOONTANKS_API APawnBase : public APawn
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    UCapsuleComponent* CapsuleComponent{nullptr};

    UPROPERTY(EditAnywhere, Category="Effects")
    UParticleSystem* DeathParticles{nullptr};

    UPROPERTY(EditAnywhere, Category="Sounds")
    USoundBase* DeathSound{nullptr};

    UPROPERTY(EditAnywhere, Category="Effects")
    TSubclassOf<UCameraShake> DeathShake;

    void StopChargingSound() const;

public:
    APawnBase();

    virtual void HandleDestruction();

    UFUNCTION(BlueprintCallable)
    float GetCurrentHealth() const;

    UFUNCTION(BlueprintCallable)
    float GetMaximumHealth() const;

protected:

    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* BaseMesh{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* TurretMesh{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USceneComponent* ProjectileSpawnPoint{nullptr};

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile Type", meta=
        (AllowPrivateAccess="true"))
    TSubclassOf<AProjectileBase> ProjectileClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UHealthComponent* HealthComponent{nullptr};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
    float FireRate{2.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
    float FireChargeDelay{0.00001f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
    UAudioComponent* FireChargeSound{nullptr};

    bool bReadyToFire{true};

    FTimerHandle FireRateHandle;

    FTimerHandle PreFireHandle;

    virtual void RotateTurret(FVector TargetLocation);

    virtual void PreFire();

    virtual void Fire();

    void RestoreFireAbility() { bReadyToFire = true; }
};
