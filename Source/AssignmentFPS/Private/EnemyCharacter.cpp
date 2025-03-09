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

	// �������������ʹ�����е� TP_WeaponComponent���ڵ�����Ҳ����ֱ��ʹ�ã�
	WeaponComponent = CreateDefaultSubobject<UTP_WeaponComponent>(TEXT("WeaponComponent"));
	// ע�⣺���ڵ��ˣ����ǲ�������룬������ AI �߼���ֱ�ӵ��� Fire()

	// ����Ҫ������ɫ�ƶ��ٶȣ������������� GetCharacterMovement() ����
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ���������ӵ����˵Ĺ����ϣ������������ "WeaponSocket" ��ۣ�
	if (WeaponComponent)
	{
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
		WeaponComponent->AttachToComponent(GetMesh(), AttachRules, FName("WeaponSocket"));
		// ����������� Character ָ��ָ���Լ�
		WeaponComponent->Character = this;
	}
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ��ȡ��ң������������Ϊ0��
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		return;
	}

	// ������������֮��ľ���
	float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());

	// �õ���ʼ�ճ������
	FVector Direction = (PlayerPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FRotator LookRotation = Direction.Rotation();
	SetActorRotation(LookRotation);

	if (Distance > AttackRange)
	{
		// ���������ڹ�����Χ�����˳�����״̬��������ƶ�
		bInAttackRange = false;
		MoveToPlayer();

		// ���֮ǰ������������ʱ���������
		GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	}
	else
	{
		// ���빥����Χ
		if (!bInAttackRange)
		{
			bInAttackRange = true;
			StopMovement();
			// ����������ʱ�����ӳ�1���ÿ1�빥��һ��
			StartAttackTimer();
		}
	}
}

void AEnemyCharacter::MoveToPlayer()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		// ʹ�ü�Ѱ·��Ҫ��ؿ������ú� Nav Mesh Volume��
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
	// �ӳ�1���ÿ1����� AttackPlayer
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEnemyCharacter::AttackPlayer, 1.0f, true, 1.0f);
}

void AEnemyCharacter::AttackPlayer()
{
	if (WeaponComponent && bCanAttack)
	{
		// ������������� Fire() ����
		WeaponComponent->Fire();
		// �����ɶ�ʱ����֤ÿ�����һ�Σ��������������ȴ
	}
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	// �ɵ��ø��ദ��
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health -= DamageAmount;

	// ÿ���ܻ���20Ѫ������ DamageAmount Ӧ��Ϊ20������Ѫ�����ڵ���0ʱ���ٵ���
	if (Health <= 0.f)
	{
		// �˴�����չ����������Ч��������Ϸʤ���߼���
		Destroy();
	}

	return ActualDamage;
}

void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// ���˲���Ҫ�����
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
