#include "EnemyCharacter.h"
#include "F:\UnrealProjects\AssignmentFPS\Source\AssignmentFPS\TP_WeaponComponent.h"
#include "Kismet/GameplayStatics.h"
#include "F:\epic\UE_5.3\Engine\Source\Runtime\AIModule\Classes\Blueprint\AIBlueprintHelperLibrary.h"
#include "TimerManager.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "F:\UnrealProjects\AssignmentFPS\Source\AssignmentFPS\AssignmentFPSGameMode.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 100.f;
	Health = 100.f;
	AttackRange = 800.f;
	bCanAttack = true;
	bInAttackRange = false;

	// 创建武器组件，使用已有的 TP_WeaponComponent
	WeaponComponent = CreateDefaultSubobject<UTP_WeaponComponent>(TEXT("WeaponComponent"));
	
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 将武器附加到敌人的骨骼上
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


	// 获取玩家
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

	UE_LOG(LogTemp, Log, TEXT("Distance to player: %f"), Distance);
}

void AEnemyCharacter::MoveToPlayer()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		// 使用简单寻路
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

	}
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health -= DamageAmount;

	// 每次受击扣20血，当血量低于等于0时销毁敌人
	if (Health <= 0.f)
	{
		AAssignmentFPSGameMode* GM = Cast<AAssignmentFPSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GM)
		{
			GM->Victory();  // 在 GameMode 中实现 Victory() 函数，用于处理胜利逻辑
		}
		Destroy();
	}


	return ActualDamage;
}

void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
