// Mattia De Prisco 2020

#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APawnTank::APawnTank()
{
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(
        TEXT("Spring arm"));
    SpringArmComponent->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArmComponent);
    FireRate = 0.5f;
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
    Super::BeginPlay();
    PlayerControllerRef = Cast<APlayerController>(GetController());
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
}

void APawnTank::CalculateMoveInput(const float Value)
{
    MoveDirection = {
        Value * MovementSpeed * GetWorld()->DeltaTimeSeconds, 0.f, 0.f
    };
}

void APawnTank::CalculateRotationInput(const float Value)
{
    const float RotateAmount{
        Value * RotationSpeed * GetWorld()->DeltaTimeSeconds
    };
    const FRotator Rotation{0.f, RotateAmount, 0.f};
    RotationDirection = FQuat{Rotation};
}

void APawnTank::Move()
{
    AddActorLocalOffset(MoveDirection, true);
}

void APawnTank::Rotate()
{
    AddActorLocalRotation(RotationDirection, true);
}

void APawnTank::Fire()
{
    if (bReadyToFire)
    {
        Super::Fire();
        GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(FireShake, 0.2f);
        bReadyToFire = false;
        FTimerHandle FireRateHandle;
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
