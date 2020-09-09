// Mattia De Prisco 2020

#include "PowerUpBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
APowerUpBase::APowerUpBase()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    HitBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PowerUp HitBox"));
    SetRootComponent(HitBox);
    HitBox->OnComponentBeginOverlap.AddDynamic(this, &APowerUpBase::OnOverlap);

    PowerUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("PowerUp Mesh"));
    PowerUpMesh->SetupAttachment(HitBox);

    PowerUpEffect = CreateDefaultSubobject<UParticleSystemComponent>(
        TEXT("PowerUp Effect"));
    PowerUpEffect->SetupAttachment(PowerUpMesh);
}

// Called when the game starts or when spawned
void APowerUpBase::BeginPlay()
{
    Super::BeginPlay();
    PlayerActor = Cast<AActor>(UGameplayStatics::GetPlayerPawn(this, 0));
    UE_LOG(LogTemp, Warning, TEXT("START!"));

}

void APowerUpBase::OnOverlap(UPrimitiveComponent* OverlappedComponent,
                           AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep,
                           const FHitResult& SweepResult)
{
    if(OtherActor == PlayerActor)
    {
        Empower();
        Destroy();
    }
}

void APowerUpBase::Empower()
{
}

