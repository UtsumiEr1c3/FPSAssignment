#include "EnemyCharacter.h"
#include "F:\UnrealProjects\AssignmentFPS\Source\AssignmentFPS\TP_WeaponComponent.h"
#include "Kismet/GameplayStatics.h"
#include "F:\epic\UE_5.3\Engine\Source\Runtime\AIModule\Classes\Blueprint\AIBlueprintHelperLibrary.h"
#include "TimerManager.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Health = 100.f;
	AttackRange = 800.f;
	bCanAttack = true;
	bInAttackRange = false;

	// 创建武器组件，使用已有的 TP_WeaponComponent（在敌人中也可以直接使用）
	WeaponComponent = CreateDefaultSubobject<UTP_WeaponComponent>(TEXT("WeaponComponent"));
	// 注意：对于敌人，我们不会绑定输入，而是在 AI 逻辑中直接调用 Fire()

	// 若需要调整角色移动速度，可在这里设置 GetCharacterMovement() 参数
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 将武器附加到敌人的骨骼上（假设骨骼上有 "WeaponSocket" 插槽）
	if (WeaponComponent)
	{
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
		WeaponComponent->AttachToComponent(GetMesh(), AttachRules, FName("WeaponSocket"));
		// 将武器组件的 Character 指针指向自己
		WeaponComponent->Character = this;
	}
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 获取玩家（假设玩家索引为0）
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		return;
	}

	// 计算玩家与敌人之间的距离
	float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());

	// 让敌人始终朝向玩家
	FVector Direction = (PlayerPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FRotator LookRotation = Direction.Rotation();
	SetActorRotation(LookRotation);

	if (Distance > AttackRange)
	{
		// 如果距离大于攻击范围，则退出攻击状态，向玩家移动
		bInAttackRange = false;
		MoveToPlayer();

		// 如果之前启动过攻击计时器，清除它
		GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	}
	else
	{
		// 进入攻击范围
		if (!bInAttackRange)
		{
			bInAttackRange = true;
			StopMovement();
			// 启动攻击定时器：延迟1秒后每1秒攻击一次
			StartAttackTimer();
		}
	}
}

void AEnemyCharacter::MoveToPlayer()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		// 使用简单寻路（要求关卡中设置好 Nav Mesh Volume）
		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), PlayerPawn);
	}
}

void AEnemyCharacter::StopMovement()
{
	if (AController* AIController = GetController())
	{
		AIController->StopMovement();
	}
}

void AEnemyCharacter::StartAttackTimer()
{
	// 延迟1秒后，每1秒调用 AttackPlayer
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEnemyCharacter::AttackPlayer, 1.0f, true, 1.0f);
}

void AEnemyCharacter::AttackPlayer()
{
	if (WeaponComponent && bCanAttack)
	{
		// 调用武器组件的 Fire() 开火
		WeaponComponent->Fire();
		// 这里由定时器保证每秒调用一次，无需额外设置冷却
	}
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	// 可调用父类处理
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health -= DamageAmount;

	// 每次受击扣20血（这里 DamageAmount 应该为20），当血量低于等于0时销毁敌人
	if (Health <= 0.f)
	{
		// 此处可扩展播放死亡特效、触发游戏胜利逻辑等
		Destroy();
	}

	return ActualDamage;
}

void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// 敌人不需要输入绑定
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
