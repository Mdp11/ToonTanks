// Copyrights Mattia De Prisco 2020

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

    UFUNCTION()
    virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent,
                           AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep,
                           const FHitResult& SweepResult);

    void RotateMesh() const;

public:
    APowerUpBase();

    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USoundBase* PickUpSound{nullptr};

    UPROPERTY()
    AActor* PlayerActor{nullptr};

    UPROPERTY(EditAnywhere)
    float BoostDuration{10.f};

    FTimerHandle BoostTimer;

    virtual void BeginPlay() override;

    virtual void Empower();
};
