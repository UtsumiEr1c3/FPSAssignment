#include "AssignmentFPSGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AAssignmentFPSGameMode::AAssignmentFPSGameMode()
{
	// ������������ VictoryWidgetClass ��Ĭ��ֵ��Ҳ������ͼ��ָ��
}

void AAssignmentFPSGameMode::Victory()
{
	// ��������ʾʤ�� UI Widget
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
