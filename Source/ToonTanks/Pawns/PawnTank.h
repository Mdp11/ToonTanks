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
    //COMPONENTS
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    USpringArmComponent* SpringArmComponent{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    UCameraComponent* CameraComponent{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
        meta=(AllowPrivateAccess="true"))
    UParticleSystemComponent* ShieldEffect{nullptr};

    //VARIABLES
    UPROPERTY()
    APlayerController* PlayerControllerRef{nullptr};

    FVector MoveDirection{};
    FQuat RotationDirection{};

    FTimerHandle FireRateHandle;

    UPROPERTY(EditAnywhere, Category="Effects")
    TSubclassOf<UCameraShake> FireShake;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(
        AllowPrivateAccess="true"))
    float DefaultMovementSpeed{550.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(
        AllowPrivateAccess="true"))
    float DefaultRotationSpeed{250.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(
        AllowPrivateAccess="true"))
    float CurrentMovementSpeed{550.f};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(
        AllowPrivateAccess="true"))
    float CurrentRotationSpeed{250.f};

    //FUNCTIONS
    bool bIsPlayerAlive{true};

    bool bReadyToFire{true};

    bool bShieldActive{false};

    void CalculateMoveInput(float Value);
    void CalculateRotationInput(float Value);

    void Move();
    void Rotate();

    void RestoreFireAbility() { bReadyToFire = true; }

    void ActivateShield();

    void DeactivateShield();

    void ImpareMovement();

    void RestoreMovement();
public:
    APawnTank();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent) override;

    virtual void HandleDestruction() override;

    bool IsAlive() const { return bIsPlayerAlive; }
	
	bool IsShieldActive() const { return bShieldActive; }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void Fire() override;
};
