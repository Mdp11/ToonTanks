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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
        meta=(AllowPrivateAccess="true"))
    UParticleSystemComponent* RightBoostEffect{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
        meta=(AllowPrivateAccess="true"))
    UParticleSystemComponent* LeftBoostEffect{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
        meta=(AllowPrivateAccess="true"))
    UAudioComponent* IdleSound{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
        meta=(AllowPrivateAccess="true"))
    UAudioComponent* MovingSound{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
        meta=(AllowPrivateAccess="true"))
    UAudioComponent* BoostSound{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Effects",
        meta=(AllowPrivateAccess="true"))
    UAudioComponent* ShieldActiveSound{nullptr};

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills", meta=(
        AllowPrivateAccess="true"))
    float MaximumShield{100.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills", meta=(
        AllowPrivateAccess="true"))
    float CurrentShield{100.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills", meta=(
        AllowPrivateAccess="true"))
    float MaximumBoost{100.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skills", meta=(
        AllowPrivateAccess="true"))
    float CurrentBoost{100.f};

    //FUNCTIONS
    bool bIsPlayerAlive{true};

    bool bShieldActive{false};

    bool bBoostActive{false};

    void CalculateMoveInput(float Value);
    void CalculateRotationInput(float Value);

    FVector CalculateBoostedMoveInput() const;

    bool Move();
    bool Rotate();

    void ActivateShield();
    void DeactivateShield();
    void ManageCurrentShield(float DeltaTime);

    void ImpairMovement();
    void RestoreMovement();
    void BoostMovement();

    void ActivateBoost();
    void DeactivateBoost();
    void ManageCurrentBoost(float DeltaTime);

    void PlayMovingSound() const;
    void StopMovingSound() const;

    void DeactivateAllSounds() const;

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

    UFUNCTION(BlueprintCallable)
    float GetCurrentShield() const { return CurrentShield; }

    UFUNCTION(BlueprintCallable)
    float GetMaximumShield() const { return MaximumShield; }

    UFUNCTION(BlueprintCallable)
    float GetCurrentBoost() const { return CurrentBoost; }

    UFUNCTION(BlueprintCallable)
    float GetMaximumBoost() const { return MaximumBoost; }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void PreFire() override;
};
