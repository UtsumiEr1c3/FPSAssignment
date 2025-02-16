#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AssignmentFPSProjectile.generated.h"

UCLASS()
class ASSIGNMENTFPS_API AAssignmentFPSProjectile : public AActor
{
    GENERATED_BODY()

public:
    AAssignmentFPSProjectile();

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
        class USphereComponent* CollisionComp;

    UPROPERTY(VisibleAnywhere, Category = "Movement")
        class UProjectileMovementComponent* ProjectileMovement;

    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
            FVector NormalImpulse, const FHitResult& Hit);
};
