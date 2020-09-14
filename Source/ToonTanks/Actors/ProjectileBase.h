// Copyrights Mattia De Prisco 2020

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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UProjectileMovementComponent* ProjectileMovement{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* ProjectileMesh{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UParticleSystemComponent* ProjectileTrailEffect{nullptr};

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

    UFUNCTION()
    virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                       UPrimitiveComponent* OtherComponent,
                       FVector NormalImpulse, const FHitResult& HitResult);

public:
    AProjectileBase();

protected:

    virtual void BeginPlay() override;
};
