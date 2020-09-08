// Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "PowerUpBase.generated.h"

class UCapsuleComponent;

UCLASS()
class TOONTANKS_API APowerUpBase : public AActor
{
    GENERATED_BODY()

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    UCapsuleComponent* HitBox{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    UStaticMeshComponent* PowerUpMesh{nullptr};

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(
        AllowPrivateAccess="true"))
    UParticleSystemComponent* PowerUpEffect{nullptr};

    UPROPERTY()
    AActor* PlayerActor{nullptr};

public:
    // Sets default values for this actor's properties
    APowerUpBase();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UFUNCTION()
    virtual void Empower(UPrimitiveComponent* OverlappedComponent,
                         AActor* OtherActor, UPrimitiveComponent* OtherComp,
                         int32 OtherBodyIndex, bool bFromSweep,
                         const FHitResult& SweepResult);
};
