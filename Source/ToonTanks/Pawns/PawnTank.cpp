// Mattia De Prisco 2020

#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"

APawnTank::APawnTank()
{
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(
        TEXT("Spring arm"));
    SpringArmComponent->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArmComponent);

    ShieldEffect = CreateDefaultSubobject<UParticleSystemComponent>
        (TEXT("Shield effect"));
    ShieldEffect->SetupAttachment(RootComponent);
    ShieldActiveSound = CreateDefaultSubobject<UAudioComponent>(
        TEXT("Shield active sound"));
    ShieldActiveSound->bAutoActivate = false;

    RightBoostEffect = CreateDefaultSubobject<UParticleSystemComponent>
        (TEXT("Right boost Effect"));
    RightBoostEffect->SetupAttachment(BaseMesh);
    LeftBoostEffect = CreateDefaultSubobject<UParticleSystemComponent>
        (TEXT("Left boost effect"));
    LeftBoostEffect->SetupAttachment(BaseMesh);
    BoostSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Boost sound"));
    BoostSound->bAutoActivate = false;

    FireRate = 0.5f;
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
    Super::BeginPlay();
    PlayerControllerRef = Cast<APlayerController>(GetController());
    ShieldEffect->Deactivate();
    RightBoostEffect->Deactivate();
    LeftBoostEffect->Deactivate();
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    Rotate();
    Move();

    if (PlayerControllerRef)
    {
        FHitResult HitResult;
        PlayerControllerRef->GetHitResultUnderCursor(
            ECC_Visibility, false, HitResult);
        RotateTurret(HitResult.ImpactPoint);
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
        BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);
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

void APawnTank::Move()
{
    if (bBoostActive)
    {
        const FVector MoveDirectionBoosted{
            CurrentMovementSpeed * GetWorld()
            ->DeltaTimeSeconds,
            0.f, 0.f
        };
        AddActorLocalOffset(MoveDirectionBoosted, true);
    }
    else
    {
        AddActorLocalOffset(MoveDirection, true);
    }
}

void APawnTank::Rotate()
{
    AddActorLocalRotation(RotationDirection, true);
}

void APawnTank::ActivateShield()
{
    if (bBoostActive)
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

void APawnTank::ActivateBoost()
{
    if (bShieldActive)
    {
        return;
    }
    bBoostActive = true;
    CurrentMovementSpeed = DefaultMovementSpeed * 3;
    CurrentRotationSpeed = DefaultRotationSpeed / 3;
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
    bBoostActive = false;
    CurrentMovementSpeed = DefaultMovementSpeed;
    CurrentRotationSpeed = DefaultRotationSpeed;
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

void APawnTank::Fire()
{
    if (bReadyToFire && !bShieldActive && !bBoostActive)
    {
        Super::Fire();
        GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(
            FireShake, 0.2f);
        bReadyToFire = false;
        GetWorld()->GetTimerManager().SetTimer(FireRateHandle, this,
                                               &APawnTank::RestoreFireAbility,
                                               FireRate);
    }
}

void APawnTank::HandleDestruction()
{
    Super::HandleDestruction();
    bIsPlayerAlive = false;
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}
