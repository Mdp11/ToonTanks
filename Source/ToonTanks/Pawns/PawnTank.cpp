// Copyrights Mattia De Prisco 2020

#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "ToonTanks/Components/HealthComponent.h"

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

    ShieldEffect = CreateDefaultSubobject<UParticleSystemComponent>
        (TEXT("Shield effect"));
    ShieldEffect->SetupAttachment(RootComponent);
    ShieldActiveSound = CreateDefaultSubobject<UAudioComponent>(
        TEXT("Shield active sound"));
    ShieldActiveSound->SetAutoActivate(false);

    RightBoostEffect = CreateDefaultSubobject<UParticleSystemComponent>
        (TEXT("Right boost Effect"));
    RightBoostEffect->SetupAttachment(BaseMesh);
    LeftBoostEffect = CreateDefaultSubobject<UParticleSystemComponent>
        (TEXT("Left boost effect"));
    LeftBoostEffect->SetupAttachment(BaseMesh);
    BoostSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Boost sound"));
    BoostSound->SetAutoActivate(false);

    FireRate = 0.5f;
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
    Super::BeginPlay();
    PlayerControllerRef = Cast<APlayerController>(GetController());

    DefaultProjectileClass = ProjectileClass;

    ShieldEffect->Deactivate();
    RightBoostEffect->Deactivate();
    LeftBoostEffect->Deactivate();

    if (!GetWorld()->GetName().Equals("MainMenu"))
    {
        IdleSound->Activate();
    }
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    const bool Moving = Move();
    const bool Rotating = Rotate();

    Moving || Rotating ? PlayMovingSound() : StopMovingSound();

    ManageCurrentShield(DeltaTime);
    ManageCurrentBoost(DeltaTime);

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

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this,
                                   &APawnTank::CalculateMoveInput);
    PlayerInputComponent->BindAxis("Turn", this,
                                   &APawnTank::CalculateRotationInput);
    PlayerInputComponent->
        BindAction("Fire", IE_Pressed, this, &APawnTank::ActivateFire);
    PlayerInputComponent->
        BindAction("Fire", IE_Released, this, &APawnTank::DeactivateFire);
    PlayerInputComponent->
        BindAction("Shield", IE_Pressed, this, &APawnTank::ActivateShield);
    PlayerInputComponent->
        BindAction("Shield", IE_Released, this, &APawnTank::DeactivateShield);
    PlayerInputComponent->
        BindAction("Boost", IE_Pressed, this, &APawnTank::ActivateBoost);
    PlayerInputComponent->
        BindAction("Boost", IE_Released, this, &APawnTank::DeactivateBoost);
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
                                          0.f,
                                          MaximumShield)) == 0.f)
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

void APawnTank::ActivateBoost()
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

void APawnTank::DeactivateBoost()
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

void APawnTank::ManageCurrentBoost(float DeltaTime)
{
    if (bBoostActive)
    {
        if ((CurrentBoost = FMath::Clamp(CurrentBoost - (DeltaTime * 20.f), 0.f,
                                         MaximumBoost)) == 0.f)
        {
            DeactivateBoost();
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
        Super::PreFire();
        GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(
            FireShake, 0.2f);
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

void APawnTank::Heal(const float HealValue) const
{
    if (HealthComponent)
    {
        HealthComponent->Heal(HealValue);
    }
}

void APawnTank::AdjustFireRate(const float FireRateMultiplier)
{
    FireRate *= FireRateMultiplier;
 
    if (GetWorld()->GetTimerManager().GetTimerRemaining(FireRateHandle) >
        FireRate)
    {
        GetWorld()->GetTimerManager().SetTimer(FireRateHandle, this,
                                               &APawnTank::RestoreFireAbility,
                                               FireRate - FireChargeDelay);
    }
}

void APawnTank::HandleDestruction()
{
    Super::HandleDestruction();
    bIsPlayerAlive = false;
    DeactivateAllSounds();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}
