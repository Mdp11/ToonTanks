// Mattia De Prisco 2020

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

    //COMPONENTS
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    UCapsuleComponent* CapsuleComponent{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    UStaticMeshComponent* BaseMesh{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    UStaticMeshComponent* TurretMesh{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    USceneComponent* ProjectileSpawnPoint{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    UHealthComponent* HealthComponent{nullptr};

    //VARIABLES
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile Type", meta=
        (AllowPrivateAccess="true"))
    TSubclassOf<AProjectileBase> ProjectileClass;

    UPROPERTY(EditAnywhere, Category="Effects")
    UParticleSystem* DeathParticles{nullptr};

    UPROPERTY(EditAnywhere, Category="Sounds")
    USoundBase* ExplosionSound{nullptr};

public:
    // Sets default values for this pawn's properties
    APawnBase();

    virtual void HandleDestruction();

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat", meta=(
        AllowPrivateAccess="true"))
    float FireRate{2.f};

    virtual void RotateTurret(FVector TargetLocation);

    virtual void Fire();
};
