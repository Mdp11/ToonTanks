// Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;

UCLASS()
class TOONTANKS_API AProjectileBase : public AActor
{
    GENERATED_BODY()

protected:
    //COMPONENTS
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UProjectileMovementComponent* ProjectileMovement{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* ProjectileMesh{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UParticleSystemComponent* ProjectileTrailEffect{nullptr};

    //VARIABLES
    UPROPERTY(EditDefaultsOnly, Category="Damage")
    TSubclassOf<UDamageType> DamageType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
    float MovementSpeed{2200.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
    float Damage{15.f};

    UPROPERTY(EditAnywhere, Category="Effects")
    UParticleSystem* ProjectileHitEffect{nullptr};

    UPROPERTY(EditAnywhere, Category="Sounds")
    USoundBase* LaunchSound{nullptr};

    UPROPERTY(EditAnywhere, Category="Sounds")
    USoundBase* HitSound{nullptr};

    UPROPERTY(EditAnywhere, Category="Sounds")
    USoundBase* ShieldHitSound{nullptr};

    //FUNCTIONS
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
               UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const
               FHitResult& HitResult);

public:
    // Sets default values for this actor's properties
    AProjectileBase();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};
