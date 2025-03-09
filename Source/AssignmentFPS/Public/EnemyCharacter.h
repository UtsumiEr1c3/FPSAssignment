#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

// 前置声明
class UTP_WeaponComponent;

UCLASS()
class ASSIGNMENTFPS_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// 使用 Nav Mesh 寻路移动到玩家 
	void MoveToPlayer();

	// 停止移动 
	void StopMovement();

	// 开火攻击玩家 
	void AttackPlayer();

	// 开始攻击计时器：等待1秒后，每秒调用 AttackPlayer 
	void StartAttackTimer();

	// 用于管理攻击定时器 
	FTimerHandle AttackTimerHandle;

	// 标记当前是否已经进入攻击状态（即距离小于或等于 AttackRange） 
	bool bInAttackRange;

public:
	// 敌人初始生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float MaxHealth;

	// 敌人当前生命值 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		float Health;

	// 攻击范围（当玩家距离小于或等于此值时停止移动并开火），单位 800 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		float AttackRange;

	// 敌人使用的武器组件 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		UTP_WeaponComponent* WeaponComponent;

	// 敌人是否允许开火（由定时器控制，每秒一次） 
	bool bCanAttack;

	// 重写受伤函数，扣血逻辑：每次扣 20 点，血量归零则销毁
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	// 敌人不需要输入绑定
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
