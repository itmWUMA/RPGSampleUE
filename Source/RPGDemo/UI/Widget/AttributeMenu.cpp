// Copyright 2024 Itmwuma


#include "UI/Widget/AttributeMenu.h"

#include "Core/RPGGameMode.h"

void UAttributeMenu::CloseAttributeMenu()
{
	const UWorld* World = GetWorld();
	if (ARPGGameMode* GameMode = World->GetAuthGameMode<ARPGGameMode>())
	{
		GameMode->CloseAttributeMenu();
	}
}

void UAttributeMenu::OnViewModelInitialized_Implementation()
{
	Super::OnViewModelInitialized_Implementation();
}
