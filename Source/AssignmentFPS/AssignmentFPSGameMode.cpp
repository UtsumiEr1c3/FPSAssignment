#include "AssignmentFPSGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AAssignmentFPSGameMode::AAssignmentFPSGameMode()
{
	// 可在这里设置 VictoryWidgetClass 的默认值，也可在蓝图中指定
}

void AAssignmentFPSGameMode::Victory()
{
	// 创建并显示胜利 UI Widget
	if (VictoryWidgetClass)
	{
		UUserWidget* VictoryWidget = CreateWidget<UUserWidget>(GetWorld(), VictoryWidgetClass);
		if (VictoryWidget)
		{
			VictoryWidget->AddToViewport();
		}
	}

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PC->SetPause(true);
	}

}
