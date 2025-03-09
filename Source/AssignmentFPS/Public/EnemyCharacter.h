#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

// ǰ������
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

	// ʹ�� Nav Mesh Ѱ·�ƶ������ 
	void MoveToPlayer();

	// ֹͣ�ƶ� 
	void StopMovement();

	// ���𹥻���� 
	void AttackPlayer();

	// ��ʼ������ʱ�����ȴ�1���ÿ����� AttackPlayer 
	void StartAttackTimer();

	// ���ڹ�������ʱ�� 
	FTimerHandle AttackTimerHandle;

	// ��ǵ�ǰ�Ƿ��Ѿ����빥��״̬��������С�ڻ���� AttackRange�� 
	bool bInAttackRange;

public:
	// ���˳�ʼ����ֵ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float MaxHealth;

	// ���˵�ǰ����ֵ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		float Health;

	// ������Χ������Ҿ���С�ڻ���ڴ�ֵʱֹͣ�ƶ������𣩣���λ 800 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		float AttackRange;

	// ����ʹ�õ�������� 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		UTP_WeaponComponent* WeaponComponent;

	// �����Ƿ��������ɶ�ʱ�����ƣ�ÿ��һ�Σ� 
	bool bCanAttack;

	// ��д���˺�������Ѫ�߼���ÿ�ο� 20 �㣬Ѫ������������
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	// ���˲���Ҫ�����
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
