// Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "PawnBase.generated.h"

class UCapsuleComponent;

UCLASS()
class TOONTANKS_API APawnBase : public APawn
{
    GENERATED_BODY()

    UPROPERTY()
    UCapsuleComponent* CapsuleComponent{nullptr};

    UPROPERTY()
    UStaticMeshComponent* BaseMesh{nullptr};

    UPROPERTY()
    UStaticMeshComponent* TurretMesh{nullptr};

    UPROPERTY()
    USceneComponent* ProjectileSpawnPoint{nullptr};

public:
    // Sets default values for this pawn's properties
    APawnBase();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};
