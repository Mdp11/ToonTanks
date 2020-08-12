// Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "PawnBase.generated.h"

class UCapsuleComponent;
class AProjectileBase;

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

    //VARIABLES
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile Type", meta=
        (AllowPrivateAccess="true"))
    TSubclassOf<AProjectileBase> ProjectileClass;

public:
    // Sets default values for this pawn's properties
    APawnBase();

protected:
    virtual void RotateTurret(FVector TargetLocation);

    virtual void Fire();

    virtual void HandleDestruction();
};
