// Mattia De Prisco 2020

#pragma once

#include "CoreMinimal.h"
#include "ToonTanks/Pawns/PawnBase.h"
#include "PawnTank.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class TOONTANKS_API APawnTank : public APawnBase
{
    GENERATED_BODY()

private:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
    USpringArmComponent* SpringArmComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
    UCameraComponent* CameraComponent;
    
    FVector MoveDirection{};
    FQuat RotationDirection{};

    float MovementSpeed{100.f};
    float RotationSpeed{100.f};

    void CalculateMoveInput(float Value);
    void CalculateRotationInput(float Value);

    void Move();
    void Rotate();

public:
    APawnTank();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};
