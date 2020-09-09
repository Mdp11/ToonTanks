// Mattia De Prisco 2020

#include "PowerUpBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
APowerUpBase::APowerUpBase()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    HitBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PowerUp HitBox"));
    SetRootComponent(HitBox);
    HitBox->OnComponentBeginOverlap.AddDynamic(this, &APowerUpBase::OnOverlap);

    PowerUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("PowerUp Mesh"));
    PowerUpMesh->SetupAttachment(HitBox);
    PowerUpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PowerUpEffect = CreateDefaultSubobject<UParticleSystemComponent>(
        TEXT("PowerUp Effect"));
    PowerUpEffect->SetupAttachment(PowerUpMesh);
}

void APowerUpBase::Tick(float DeltaTime)
{
    RotateMesh();
}

// Called when the game starts or when spawned
void APowerUpBase::BeginPlay()
{
    Super::BeginPlay();
    PlayerActor = Cast<AActor>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void APowerUpBase::OnOverlap(UPrimitiveComponent* OverlappedComponent,
                             AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             int32 OtherBodyIndex, bool bFromSweep,
                             const FHitResult& SweepResult)
{
    if (OtherActor == PlayerActor)
    {
        Empower();
        UGameplayStatics::PlaySoundAtLocation(this, PickUpSound,
                                              GetActorLocation());
    }
}

void APowerUpBase::RotateMesh()
{
    FRotator CurrentRotation = PowerUpMesh->GetComponentRotation();
    PowerUpMesh->SetWorldRotation(FMath::Lerp(
        CurrentRotation, CurrentRotation - FRotator{0.f, 1.f, 0.f}, 2.f));
}

void APowerUpBase::Empower()
{
}
