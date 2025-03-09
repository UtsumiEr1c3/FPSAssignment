#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AssignmentFPSGameMode.generated.h"

UCLASS()
class ASSIGNMENTFPS_API AAssignmentFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAssignmentFPSGameMode();

	// ��ʤ����������ʱ���øú���
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void Victory();

protected:
	// �ڱ༭����ָ��ʤ��ʱ��ʾ�� Widget Blueprint ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> VictoryWidgetClass;
};
