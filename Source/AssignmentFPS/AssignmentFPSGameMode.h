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

	// 当胜利条件满足时调用该函数
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void Victory();

protected:
	// 在编辑器中指定胜利时显示的 Widget Blueprint 类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> VictoryWidgetClass;
};
