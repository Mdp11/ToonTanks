// Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "ToonTanks/Pawns/PawnBase.h"
#include "PawnTank.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class TOONTANKS_API APawnTank : public APawnBase
{
    GENERATED_BODY()

private:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    UCameraComponent* CameraComponent;

    UPROPERTY()
    APlayerController* PlayerControllerRef{nullptr};

    FVector MoveDirection{};
    FQuat RotationDirection{};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(
        AllowPrivateAccess="true"))
    float MovementSpeed{550.f};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(
        AllowPrivateAccess="true"))
    float RotationSpeed{250.f};

    bool bIsPlayerAlive{true};

    bool bReadyToFire{true};

    void CalculateMoveInput(float Value);
    void CalculateRotationInput(float Value);

    void Move();
    void Rotate();

    void RestoreFireAbility() { bReadyToFire = true; }

public:
    APawnTank();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent) override;

    virtual void HandleDestruction() override;

    bool IsAlive() const { return bIsPlayerAlive; }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void Fire() override;
};
