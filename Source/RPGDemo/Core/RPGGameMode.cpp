// Copyright 2024 Itmwuma


#include "Core/RPGGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Level/Load/LoadManager.h"
#include "Level/Load/RPGSaveGame.h"
#include "UI/ITMUserInterfaceUtilities.h"
#include "UI/Widget/AttributeMenu.h"

void ARPGGameMode::ShowAttributeMenu()
{
	if (AttributeMenuWidget)
	{
		FITMUserInterfaceUtilities::ResumeWidget(AttributeMenuWidget);
	}
	else
	{
		UITMWidgetBase* Widget = FITMUserInterfaceUtilities::CreateRootWidget(
			this,
			AttributeMenuClass.WidgetClass,
			TEXT("AttributeMenu"),
			FITMUserInterfaceUtilities::MakeDefaultViewModelParams(this),
			AttributeMenuClass.ViewModelClass
			);
		AttributeMenuWidget = Cast<UAttributeMenu>(Widget);
	}
}

void ARPGGameMode::CloseAttributeMenu()
{
	FITMUserInterfaceUtilities::HideWidget(AttributeMenuWidget);
}

AActor* ARPGGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	ULoadManager* LoadMgr = ULoadManager::Get(GetWorld());
	if (!IsValid(LoadMgr))
		return nullptr;
		
	URPGSaveGame* CurrentSaveData = LoadMgr->LoadSlotData(LoadMgr->GetCurrentSaveSlotIndex());
	
	if (Actors.Num() > 0)
	{
		AActor* SelectedPlayerStart = Actors[0];
		for (AActor* Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (IsValid(CurrentSaveData))
				{
					if (PlayerStart->PlayerStartTag == CurrentSaveData->PlayerStartTag)
					{
						SelectedPlayerStart = PlayerStart;
						break;
					}
				}
				else
				{
					if (PlayerStart->PlayerStartTag == LoadMgr->GetDefaultPlayerStartTag())
					{
						SelectedPlayerStart = PlayerStart;
						break;
					}
				}
			}
		}
		return SelectedPlayerStart;
	}

	return nullptr;
}

void ARPGGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (ULoadManager* LoadMgr = ULoadManager::Get(this))
	{
		if (URPGSaveGame* CurSaveGame = LoadMgr->LoadSlotData(LoadMgr->GetCurrentSaveSlotIndex()))
			LoadMgr->LoadActorInfoFromSaveGame(CurSaveGame);
	}
}
