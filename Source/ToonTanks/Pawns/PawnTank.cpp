// Copyrights Mattia De Prisco 2020

#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "ToonTanks/Components/HealthComponent.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"

APawnTank::APawnTank()
{
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(
        TEXT("Spring arm"));
    SpringArmComponent->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArmComponent);

    IdleSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Idle sound"));
    IdleSound->SetAutoActivate(false);

    MovingSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Moving sound"));
    MovingSound->SetAutoActivate(false);

    HealEffect = CreateDefaultSubobject<UParticleSystemComponent>(
        TEXT("Heal effect"));
    HealEffect->SetupAttachment(RootComponent);
    HealEffect->SetAutoActivate(false);

    ShieldEffect = CreateDefaultSubobject<UParticleSystemComponent>(
        TEXT("Shield effect"));
    ShieldEffect->SetupAttachment(RootComponent);
    ShieldActiveSound = CreateDefaultSubobject<UAudioComponent>(
        TEXT("Shield active sound"));
    ShieldActiveSound->SetAutoActivate(false);

    RightBoostEffect = CreateDefaultSubobject<UParticleSystemComponent>(
        TEXT("Right boost effect"));
    RightBoostEffect->SetupAttachment(BaseMesh);
    LeftBoostEffect = CreateDefaultSubobject<UParticleSystemComponent>(
        TEXT("Left boost effect"));
    LeftBoostEffect->SetupAttachment(BaseMesh);
    BoostSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Boost sound"));
    BoostSound->SetAutoActivate(false);

    NoAmmoSound = CreateDefaultSubobject<UAudioComponent
    >(TEXT("No ammo sound"));
    NoAmmoSound->SetAutoActivate(false);

    FireRate = 0.5f;
}

void APawnTank::BeginPlay()
{
    Super::BeginPlay();

    PlayerControllerRef = Cast<APlayerController>(GetController());

    DefaultProjectileClass = ProjectileClass;
    DefaultFireRate = FireRate;

    ShieldEffect->Deactivate();
    RightBoostEffect->Deactivate();
    LeftBoostEffect->Deactivate();

    if (!GetWorld()->GetName().Equals("MainMenu"))
    {
        IdleSound->Activate();
    }

    CalculateRotationInput(0.f);
}

void APawnTank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    const bool Moving = Move();
    const bool Rotating = Rotate();

    Moving || Rotating ? PlayMovingSound() : StopMovingSound();

    ManageCurrentShield(DeltaTime);
    ManageCurrentSpeedBoost(DeltaTime);

    if (PlayerControllerRef)
    {
        FHitResult HitResult;
        PlayerControllerRef->GetHitResultUnderCursor(
            ECC_Visibility, false, HitResult);
        RotateTurret(HitResult.ImpactPoint);
    }

    if (bFiring)
    {
        PreFire();
    }
}

void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this,
                                   &APawnTank::CalculateMoveInput);
    PlayerInputComponent->BindAxis("Turn", this,
                                   &APawnTank::CalculateRotationInput);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this,
                                     &APawnTank::ActivateFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, this,
                                     &APawnTank::DeactivateFire);
    PlayerInputComponent->BindAction("Shield", IE_Pressed, this,
                                     &APawnTank::ActivateShield);
    PlayerInputComponent->BindAction("Shield", IE_Released, this,
                                     &APawnTank::DeactivateShield);
    PlayerInputComponent->BindAction("Boost", IE_Pressed, this,
                                     &APawnTank::ActivateSpeedBoost);
    PlayerInputComponent->BindAction("Boost", IE_Released, this,
                                     &APawnTank::DeactivateSpeedBoost);
    PlayerInputComponent->BindAction("Menu", IE_Pressed, this,
                                     &APawnTank::HandleMenu);

    PlayerInputComponent->BindAction<FWeaponSlotInputDelegate, APawnTank, int>(
        "Weapon Slot 1", IE_Pressed, this, &APawnTank::SwitchWeaponSlot, 0);
    PlayerInputComponent->BindAction<FWeaponSlotInputDelegate, APawnTank, int>(
        "Weapon Slot 2", IE_Pressed, this, &APawnTank::SwitchWeaponSlot, 1);
    PlayerInputComponent->BindAction<FWeaponSlotInputDelegate, APawnTank, int>(
        "Weapon Slot 3", IE_Pressed, this, &APawnTank::SwitchWeaponSlot, 2);
    PlayerInputComponent->BindAction<FWeaponSlotInputDelegate, APawnTank, int>(
        "Weapon Slot 4", IE_Pressed, this, &APawnTank::SwitchWeaponSlot, 3);
}

void APawnTank::CalculateMoveInput(const float Value)
{
    MoveDirection = {
        Value * CurrentMovementSpeed * GetWorld()->DeltaTimeSeconds, 0.f, 0.f
    };
}

void APawnTank::CalculateRotationInput(const float Value)
{
    const float RotateAmount{
        Value * CurrentRotationSpeed * GetWorld()->DeltaTimeSeconds
    };
    const FRotator Rotation{0.f, RotateAmount, 0.f};
    RotationDirection = FQuat{Rotation};
}

FVector APawnTank::CalculateBoostedMoveInput() const
{
    return {CurrentMovementSpeed * GetWorld()->DeltaTimeSeconds, 0.f, 0.f};
}

bool APawnTank::Move()
{
    if (bBoostActive)
    {
        AddActorLocalOffset(CalculateBoostedMoveInput(), true);
    }
    else
    {
        AddActorLocalOffset(MoveDirection, true);
    }

    const bool IsMoving = MoveDirection.X != 0 || bBoostActive;

    return IsMoving;
}

bool APawnTank::Rotate()
{
    AddActorLocalRotation(RotationDirection, true);

    const bool IsRotating = !FMath::IsNearlyEqual(
        RotationDirection.Z, 0.f, 0.01f);

    return IsRotating;
}

void APawnTank::ActivateShield()
{
    if (bBoostActive || CurrentShield <= 25.f)
    {
        return;
    }

    bShieldActive = true;
    ImpairMovement();

    if (ShieldEffect)
    {
        ShieldEffect->Activate();
    }
    if (ShieldActiveSound)
    {
        ShieldActiveSound->Play();
    }
}

void APawnTank::DeactivateShield()
{
    if (bBoostActive)
    {
        return;
    }

    bShieldActive = false;
    RestoreMovement();

    if (ShieldEffect)
    {
        ShieldEffect->Deactivate();
    }
    if (ShieldActiveSound)
    {
        ShieldActiveSound->Stop();
    }
}

void APawnTank::ManageCurrentShield(float DeltaTime)
{
    if (bShieldActive)
    {
        if ((CurrentShield = FMath::Clamp(CurrentShield - (DeltaTime * 20.f),
                                          0.f, MaximumShield)) == 0.f)
        {
            DeactivateShield();
        }
    }
    else
    {
        CurrentShield = FMath::Clamp(CurrentShield + (DeltaTime * 10.f), 0.f,
                                     MaximumShield);
    }
}

void APawnTank::ImpairMovement()
{
    CurrentMovementSpeed = DefaultMovementSpeed / 3;
    CurrentRotationSpeed = DefaultRotationSpeed / 3;
}

void APawnTank::RestoreMovement()
{
    CurrentMovementSpeed = DefaultMovementSpeed;
    CurrentRotationSpeed = DefaultRotationSpeed;
}

void APawnTank::BoostMovement()
{
    CurrentMovementSpeed = DefaultMovementSpeed * 3;
    CurrentRotationSpeed = DefaultRotationSpeed / 3;
}

void APawnTank::ActivateSpeedBoost()
{
    if (bShieldActive || CurrentBoost <= 25.f)
    {
        return;
    }

    BoostMovement();

    bBoostActive = true;

    if (RightBoostEffect && LeftBoostEffect)
    {
        RightBoostEffect->Activate();
        LeftBoostEffect->Activate();
    }

    if (BoostSound)
    {
        BoostSound->Play();
    }
}

void APawnTank::DeactivateSpeedBoost()
{
    if (bShieldActive)
    {
        return;
    }

    RestoreMovement();

    bBoostActive = false;

    if (RightBoostEffect && LeftBoostEffect)
    {
        RightBoostEffect->Deactivate();
        LeftBoostEffect->Deactivate();
    }

    if (BoostSound)
    {
        BoostSound->Stop();
    }
}

void APawnTank::ManageCurrentSpeedBoost(float DeltaTime)
{
    if (bBoostActive)
    {
        if ((CurrentBoost = FMath::Clamp(CurrentBoost - (DeltaTime * 20.f), 0.f,
                                         MaximumBoost)) == 0.f)
        {
            DeactivateSpeedBoost();
        }
    }
    else
    {
        CurrentBoost = FMath::Clamp(CurrentBoost + (DeltaTime * 10.f), 0.f,
                                    MaximumBoost);
    }
}

void APawnTank::PreFire()
{
    if (bReadyToFire && !bShieldActive && !bBoostActive)
    {
        if (WeaponsBulletsCount[CurrentWeaponSlot] > 0)
        {
            if (CurrentWeaponSlot != 0)
            {
                WeaponsBulletsCount[CurrentWeaponSlot]--;
            }
            Super::PreFire();
            GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(
                FireShake, 0.2f);
        }
        else
        {
            if (NoAmmoSound && !NoAmmoSound->IsPlaying())
            {
                NoAmmoSound->Play();
            }
        }
    }
}

void APawnTank::SwitchWeaponSlot(const int Slot)
{
    if (Slot != CurrentWeaponSlot)
    {
        CurrentWeaponSlot = Slot;
        ProjectileClass = Weapons[Slot];
        UGameplayStatics::PlaySoundAtLocation(this, WeaponSwitchSound,
                                              GetActorLocation());
    }
}

void APawnTank::PlayMovingSound() const
{
    if (!MovingSound->IsPlaying())
    {
        MovingSound->Play();
    }
}

void APawnTank::StopMovingSound() const
{
    if (MovingSound->IsPlaying())
    {
        MovingSound->Stop();
    }
}

void APawnTank::DeactivateAllSounds() const
{
    StopMovingSound();
    IdleSound->Deactivate();
    ShieldActiveSound->Deactivate();
    BoostSound->Deactivate();
}

void APawnTank::HandleMenu()
{
    if(bMenuActive)
    {
        CloseMenu();
        
    }
    else
    {
        SpawnMenu();
    }
    bMenuActive = !bMenuActive;
}

void APawnTank::Heal(const float HealValue) const
{
    if (HealthComponent)
    {
        HealthComponent->Heal(HealValue);
        if (HealEffect)
        {
            HealEffect->Activate();
        }
    }
}

void APawnTank::BoostFireRate(const float FireRateMultiplier,
                              const float Duration)
{
    FireRate *= FireRateMultiplier;

    if (GetWorld()->GetTimerManager().GetTimerRemaining(FireRateHandle) >
        FireRate)
    {
        GetWorld()->GetTimerManager().SetTimer(FireRateHandle, this,
                                               &APawnTank::RestoreFireAbility,
                                               FireRate - FireChargeDelay);
    }

    GetWorld()->GetTimerManager().SetTimer(BoostedFireRateHandle, this,
                                           &APawnTank::RestoreFireRate,
                                           Duration);
}

void APawnTank::AddAmmo(const int WeaponType, const int AmmoAmount)
{
    WeaponsBulletsCount[WeaponType] += AmmoAmount;
}

void APawnTank::HandleDestruction()
{
    Super::HandleDestruction();
    bIsPlayerAlive = false;
    DeactivateAllSounds();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}
