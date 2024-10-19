// Copyright 2024 Itmwuma


#include "Core/RPGHUD.h"

#include "RPGGameMode.h"
#include "UI/Widget/RootHUD.h"
#include "UI/ITMUserInterfaceUtilities.h"
#include "UI/ViewModel/RootHUDViewModel.h"

void ARPGHUD::InitHUD(const FITMViewModelParams& Params)
{
	if (const UWorld* World = GetWorld())
	{
		if (const ARPGGameMode* GameMode = World->GetAuthGameMode<ARPGGameMode>())
		{
			const FITMWidgetClassConfig& ClassConfig = GameMode->GetRootHUDClass();
			UITMWidgetBase* Widget = FITMUserInterfaceUtilities::CreateRootWidget(
				this,
				ClassConfig.WidgetClass,
				TEXT("RootHUD"),
				Params,
				ClassConfig.ViewModelClass);
			HUDWidget = Cast<URootHUD>(Widget);
			if (HUDWidget)
			{
				HUDWidget->SetWidgetInputMode(true);
			}
		}
	}
}
