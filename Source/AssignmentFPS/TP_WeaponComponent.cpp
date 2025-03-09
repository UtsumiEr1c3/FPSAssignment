#include "TP_WeaponComponent.h"
#include "AssignmentFPSCharacter.h" // 玩家类
#include "AssignmentFPSProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// 默认偏移量
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
	// 如果角色是玩家，则使用玩家摄像机的旋转；如果是敌人, 使用角色正前方
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

	// 计算发射位置：将偏移量转换到世界空间
	const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	World->SpawnActor<AAssignmentFPSProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// 只对玩家播放第一人称动画
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
	// 将 Character 赋值给通用的 ACharacter* 变量
	Character = TargetCharacter;

	// 如果角色是玩家，则调用玩家专用逻辑
	if (Character->IsA(AAssignmentFPSCharacter::StaticClass()))
	{
		AAssignmentFPSCharacter* PlayerChar = Cast<AAssignmentFPSCharacter>(Character);
		// 检查玩家是否已经拥有枪
		if (PlayerChar->GetHasRifle())
		{
			return;
		}

		// 将武器附加到玩家的第一人称 Mesh 上
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		AttachToComponent(PlayerChar->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

		// 设置玩家状态
		PlayerChar->SetHasRifle(true);

		// 绑定输入（只对玩家有效）
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
		// 如果是敌人，直接附加到默认 Mesh 的 "WeaponSocket" 插槽上
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

	// 只对玩家移除输入映射
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