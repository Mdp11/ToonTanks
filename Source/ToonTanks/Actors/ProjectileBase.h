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
private:
    //COMPONENTS
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
        meta=(AllowPrivateAccess="true"))
    UProjectileMovementComponent* ProjectileMovement{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
        meta=(AllowPrivateAccess="true"))
    UStaticMeshComponent* ProjectileMesh{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",
        meta=(AllowPrivateAccess="true"))
    UParticleSystemComponent* ProjectileTrailEffect{nullptr};

    //VARIABLES
    UPROPERTY(EditDefaultsOnly, Category="Damage")
    TSubclassOf<UDamageType> DamageType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement",
        meta=(AllowPrivateAccess="true"))
    float MovementSpeed{1300.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage",
        meta=(AllowPrivateAccess="true"))
    float Damage{15.f};

    UPROPERTY(EditAnywhere, Category="Effects")
    UParticleSystem* ProjectileHitEffect{nullptr};

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
