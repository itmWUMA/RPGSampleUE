// Copyright 2024 Itmwuma


#include "UI/Widget/MainMenuWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Level/Load/LoadManager.h"
#include "UI/ITMUserInterfaceUtilities.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MainMenuMusic)
	{
		UGameplayStatics::PlaySound2D(this, MainMenuMusic);
	}
}

void UMainMenuWidget::NativeDestruct()
{
	MainMenuWidgetStack.Empty();
	
	Super::NativeDestruct();
}

void UMainMenuWidget::PushWidget(UWidget* InWidget)
{
	if (!InWidget)
		return;
	
	if (MainMenuWidgetStack.Num() > 0)
		MainMenuWidgetStack.Last()->SetVisibility(ESlateVisibility::Collapsed);
	InWidget->SetVisibility(ESlateVisibility::Visible);
	
	MainMenuWidgetStack.Push(InWidget);
}

void UMainMenuWidget::PopWidget()
{
	if (MainMenuWidgetStack.Num() <= 0)
		return;

	UWidget* PopWidget = MainMenuWidgetStack.Pop();
	PopWidget->SetVisibility(ESlateVisibility::Collapsed);
	if (MainMenuWidgetStack.Num() > 0)
		MainMenuWidgetStack.Last()->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuWidget::EnterGame()
{
}

void UMainMenuWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this,
		UGameplayStatics::GetPlayerController(this, 0),
		EQuitPreference::Quit,
		false);
}

void UMainMenuWidget::LoadGame()
{
	if (ULoadManager* LoadMgr = ULoadManager::Get(this))
		LoadMgr->ShowLoadUI();
}
