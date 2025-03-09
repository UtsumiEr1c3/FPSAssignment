#include "TP_WeaponComponent.h"
#include "AssignmentFPSCharacter.h" // �����
#include "AssignmentFPSProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Ĭ��ƫ����
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr)
	{
		return;
	}

	UWorld* const World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	FRotator SpawnRotation;
	// �����ɫ����ң���ʹ��������������ת������ǵ���, ʹ�ý�ɫ��ǰ��
	if (Character->IsA(AAssignmentFPSCharacter::StaticClass()))
	{
		AAssignmentFPSCharacter* PlayerChar = Cast<AAssignmentFPSCharacter>(Character);
		if (PlayerChar == nullptr || PlayerChar->GetController() == nullptr)
		{
			return;
		}
		APlayerController* PC = Cast<APlayerController>(PlayerChar->GetController());
		SpawnRotation = PC->PlayerCameraManager->GetCameraRotation();
	}
	else
	{
		SpawnRotation = Character->GetActorForwardVector().Rotation();
	}

	// ���㷢��λ�ã���ƫ����ת��������ռ�
	const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	World->SpawnActor<AAssignmentFPSProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// ֻ����Ҳ��ŵ�һ�˳ƶ���
	if (FireAnimation != nullptr && Character->IsA(AAssignmentFPSCharacter::StaticClass()))
	{
		AAssignmentFPSCharacter* PlayerChar = Cast<AAssignmentFPSCharacter>(Character);
		if (PlayerChar && PlayerChar->GetMesh1P())
		{
			UAnimInstance* AnimInstance = PlayerChar->GetMesh1P()->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
}

void UTP_WeaponComponent::AttachWeapon(AAssignmentFPSCharacter* TargetCharacter)
{
	// �� Character ��ֵ��ͨ�õ� ACharacter* ����
	Character = TargetCharacter;

	// �����ɫ����ң���������ר���߼�
	if (Character->IsA(AAssignmentFPSCharacter::StaticClass()))
	{
		AAssignmentFPSCharacter* PlayerChar = Cast<AAssignmentFPSCharacter>(Character);
		// �������Ƿ��Ѿ�ӵ��ǹ
		if (PlayerChar->GetHasRifle())
		{
			return;
		}

		// ���������ӵ���ҵĵ�һ�˳� Mesh ��
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		AttachToComponent(PlayerChar->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

		// �������״̬
		PlayerChar->SetHasRifle(true);

		// �����루ֻ�������Ч��
		if (APlayerController* PC = Cast<APlayerController>(PlayerChar->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(FireMappingContext, 1);
			}

			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent))
			{
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
			}
		}
	}
	else
	{
		// ����ǵ��ˣ�ֱ�Ӹ��ӵ�Ĭ�� Mesh �� "WeaponSocket" �����
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		AttachToComponent(Character->GetMesh(), AttachmentRules, FName(TEXT("WeaponSocket")));
	}
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	// ֻ������Ƴ�����ӳ��
	if (Character->IsA(AAssignmentFPSCharacter::StaticClass()))
	{
		AAssignmentFPSCharacter* PlayerChar = Cast<AAssignmentFPSCharacter>(Character);
		if (APlayerController* PC = Cast<APlayerController>(PlayerChar->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(FireMappingContext);
			}
		}
	}
}