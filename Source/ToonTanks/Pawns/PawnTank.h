// Copyrights Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "ToonTanks/Pawns/PawnBase.h"
#include "PawnTank.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TOONTANKS_API APawnTank : public APawnBase
{
    GENERATED_BODY()

private:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    USpringArmComponent* SpringArmComponent{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    UCameraComponent* CameraComponent{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    UParticleSystemComponent* HealEffect{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    UParticleSystemComponent* ShieldEffect{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    UParticleSystemComponent* RightBoostEffect{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    UParticleSystemComponent* LeftBoostEffect{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    UAudioComponent* IdleSound{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    UAudioComponent* MovingSound{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    UAudioComponent* BoostSound{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Effects", meta=(
        AllowPrivateAccess="true"))
    UAudioComponent* ShieldActiveSound{nullptr};

    UPROPERTY(EditAnywhere, Category="Sounds")
    USoundBase* WeaponSwitchSound{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Effects", meta=(
        AllowPrivateAccess="true"))
    UAudioComponent* NoAmmoSound{nullptr};

    UPROPERTY()
    APlayerController* PlayerControllerRef{nullptr};

    FVector MoveDirection{};
    FQuat RotationDirection{};

    UPROPERTY(EditAnywhere, Category="Effects")
    TSubclassOf<UMatineeCameraShake> FireShake;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(
        AllowPrivateAccess="true"))
    float DefaultMovementSpeed{550.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(
        AllowPrivateAccess="true"))
    float DefaultRotationSpeed{150.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(
        AllowPrivateAccess="true"))
    float CurrentMovementSpeed{550.f};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta=(
        AllowPrivateAccess="true"))
    float CurrentRotationSpeed{150.f};

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

    TSubclassOf<AProjectileBase> DefaultProjectileClass;

    float DefaultFireRate{0.f};

    FTimerHandle BoostedFireRateHandle;

    FTimerHandle BoostedProjectileHandle;

    bool bIsPlayerAlive{true};

    bool bFiring{false};

    bool bShieldActive{false};

    bool bBoostActive{false};

    DECLARE_DELEGATE_OneParam(FWeaponSlotInputDelegate, int);

    int CurrentWeaponSlot{0};

    bool bMenuSpawnable{true};

    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<AProjectileBase>> Weapons;
    TArray<int> WeaponsBulletsCount{1, 0, 0, 0};

    void CalculateMoveInput(float Value);
    void CalculateRotationInput(float Value);

    void ActivateFire() { bFiring = true; }
    void DeactivateFire() { bFiring = false; }

    FVector CalculateBoostedMoveInput() const;

    bool Move();
    bool Rotate();

    void ActivateShield();
    void DeactivateShield();
    void ManageCurrentShield(float DeltaTime);

    void ImpairMovement();
    void RestoreMovement();
    void BoostMovement();

    void ActivateSpeedBoost();
    void DeactivateSpeedBoost();
    void ManageCurrentSpeedBoost(float DeltaTime);

    void RestoreFireRate()
    {
        FireRate = DefaultFireRate;
    }

    void SwitchWeaponSlot(int Slot);

    void PlayMovingSound() const;
    void StopMovingSound() const;

    void DeactivateAllSounds() const;

    void HandleMenu();

public:
    APawnTank();

    virtual void Tick(float DeltaTime) override;

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
    float GetCurrentSpeedBoost() const { return CurrentBoost; }

    UFUNCTION(BlueprintCallable)
    float GetMaximumSpeedBoost() const { return MaximumBoost; }

    UFUNCTION(BlueprintCallable)
    int GetCurrentWeapon() const { return CurrentWeaponSlot; }

    UFUNCTION(BlueprintCallable)
    int GetAmmoForCurrentWeapon() const
    {
        return WeaponsBulletsCount[CurrentWeaponSlot];
    }

    void Heal(float HealValue) const;

    void BoostFireRate(float FireRateMultiplier, float Duration);

    void AddAmmo(const int WeaponType, const int AmmoAmount);

    void SetMenuSpawnable(const bool Value) { bMenuSpawnable = Value; }

    UFUNCTION(BlueprintImplementableEvent)
    void CloseMenu();

    bool IsMenuActive() const { return bMenuActive; }

protected:

    virtual void BeginPlay() override;

    virtual void PreFire() override;

    UFUNCTION(BlueprintImplementableEvent)
    void SpawnMenu();

    bool bMenuActive{false};
};
