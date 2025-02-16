#include "AssignmentFPSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h" 
#include "EnemyCharacter.h"  

AAssignmentFPSProjectile::AAssignmentFPSProjectile()
{
    // 创建碰撞组件
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);
    CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->OnComponentHit.AddDynamic(this, &AAssignmentFPSProjectile::OnHit);
    RootComponent = CollisionComp;

    // 创建子弹的移动组件
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;

    // 设置子弹寿命
    InitialLifeSpan = 3.0f;
}


void AAssignmentFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor->IsA(AEnemyCharacter::StaticClass()))
    {
        AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
        if (Enemy)
        {
            // 使用 UGameplayStatics::ApplyDamage，自动处理 FDamageEvent
            UGameplayStatics::ApplyDamage(Enemy, 20.0f, nullptr, this, UDamageType::StaticClass());
        }
    }

    Destroy(); // 碰撞后销毁子弹
}
