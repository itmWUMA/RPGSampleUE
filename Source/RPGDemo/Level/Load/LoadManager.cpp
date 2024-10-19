// Copyright 2024 Itmwuma


#include "Level/Load/LoadManager.h"

#include "EngineUtils.h"
#include "LoadSystemData.h"
#include "SaveActor.h"
#include "AbilitySystem/RPGAttributeSetBase.h"
#include "Core/RPGPlayerState.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/ITMUserInterfaceUtilities.h"
#include "UI/ViewModel/ITMViewModelBase.h"
#include "UI/Widget/LoadWidget.h"

void ULoadManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadDataAsset =
		LoadObject<ULoadSystemData>(this, TEXT("/Script/RPGDemo.LoadSystemData'/Game/RPGDemo/Blueprints/Level/Load/DA_LoadSystem.DA_LoadSystem'"));
}

void ULoadManager::ShowLoadUI()
{
	if (LoadWidgetCache)
	{
		FITMUserInterfaceUtilities::ResumeWidget(LoadWidgetCache);
	}
	else
	{
		FITMViewModelParams ViewModelParams;
		LoadWidgetCache = Cast<ULoadWidget>(FITMUserInterfaceUtilities::CreateRootWidget(
			this,
			LoadDataAsset->LoadWidgetData.WidgetClass,
			TEXT("LoadWidget"),
			ViewModelParams,
			LoadDataAsset->LoadWidgetData.ViewModelClass));
	}
}

void ULoadManager::CloseLoadUI()
{
	FITMUserInterfaceUtilities::HideWidget(LoadWidgetCache, true);
}

void ULoadManager::ShowSaveUI()
{
	if (SaveWidgetCache)
	{
		FITMUserInterfaceUtilities::ResumeWidget(SaveWidgetCache);
	}
	else
	{
		FITMViewModelParams ViewModelParams = FITMUserInterfaceUtilities::MakeDefaultViewModelParams(this);
		SaveWidgetCache = Cast<ULoadWidget>(FITMUserInterfaceUtilities::CreateRootWidget(
			this,
			LoadDataAsset->SaveWidgetData.WidgetClass,
			TEXT("SaveWidget"),
			ViewModelParams,
			LoadDataAsset->SaveWidgetData.ViewModelClass));
	}
}

void ULoadManager::CloseSaveUI()
{
	FITMUserInterfaceUtilities::HideWidget(SaveWidgetCache);
}

void ULoadManager::CreateNewGame()
{
	if (!LoadDataAsset)
		return;

	TSoftObjectPtr<UWorld> NewMap = LoadDataAsset->GetMapByName(LoadDataAsset->NewGameLevel);
	CurrentSaveSlotIndex = -1;
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, NewMap);
}

void ULoadManager::LoadLevelFromSaveGame(URPGSaveGame* InSaveGame)
{
	if (!IsValid(InSaveGame) || !LoadDataAsset)
		return;

	TSoftObjectPtr<UWorld> PendingLoadMap = LoadDataAsset->GetMapByName(InSaveGame->MapName);
	CurrentSaveSlotIndex = InSaveGame->SlotIndex;
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, PendingLoadMap);
}

FName ULoadManager::GetDefaultPlayerStartTag() const
{
	return LoadDataAsset ? LoadDataAsset->DefaultPlayerStartTag : NAME_None;
}

ULoadManager* ULoadManager::Get(const UObject* WorldContext)
{
	if (UGameInstance* GI = UGameplayStatics::GetGameInstance(WorldContext))
		return GI->GetSubsystem<ULoadManager>();
	return nullptr;
}

URPGSaveGame* ULoadManager::SaveSlotData(int32 SlotIndex)
{
	if (!LoadDataAsset)
		return nullptr;

	FString SlotName = MakeSlotNameByIndex(SlotIndex);
	
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
	
	URPGSaveGame* SaveGameObject =
		Cast<URPGSaveGame>(UGameplayStatics::CreateSaveGameObject(LoadDataAsset->SaveGameClass));
	if (IsValid(SaveGameObject))
	{
		// 存储基础存档信息
		SaveGameObject->PlayerName = FPlatformProcess::UserName();
		SaveGameObject->SlotIndex = SlotIndex;
		SaveGameObject->SlotName = SlotName;
		SaveGameObject->SlotDateTime = FDateTime::Now();
		SaveGameObject->MapName = UGameplayStatics::GetCurrentLevelName(this);

		// 存储PlayerStart信息
		if (APlayerStart* SelectedPlayerStart = GetClosestPlayerStart())
			SaveGameObject->PlayerStartTag = SelectedPlayerStart->PlayerStartTag;
		
		if (ARPGPlayerState* PS = Cast<ARPGPlayerState>(UGameplayStatics::GetPlayerState(this, 0)))
		{
			// 存储PlayerState信息
			SaveGameObject->PlayerLevel = PS->GetPlayerLevel();
			SaveGameObject->XP = PS->GetXP();
			SaveGameObject->AttributePoint = PS->GetAttributePoint();

			// 存储属性数值
			if (URPGAttributeSetBase* AttributeSet = Cast<URPGAttributeSetBase>(PS->GetAttributeSet()))
			{
				SaveGameObject->Strength = AttributeSet->GetStrength();
				SaveGameObject->Intelligence = AttributeSet->GetIntelligence();
				SaveGameObject->Resilience = AttributeSet->GetResilience();
				SaveGameObject->Vigor = AttributeSet->GetVigor();
			}
		}

		// 存储关卡和Actor数据
		if (UWorld* CurrentWorld = GetWorld())
		{
			FString WorldName = CurrentWorld->GetMapName();
			WorldName.RemoveFromStart(CurrentWorld->StreamingLevelsPrefix);
			if (!SaveGameObject->HasMap(WorldName))
			{
				FSavedMap NewSavedMap;
				NewSavedMap.MapAssetName = WorldName;
				SaveGameObject->SavedMaps.Add(NewSavedMap);
			}

			FSavedMap SavedMap = SaveGameObject->GetSavedMapWithMapName(WorldName);
			SavedMap.SavedActors.Empty();
			for (FActorIterator It(CurrentWorld); It; ++It)
			{
				AActor* Actor = *It;
				if (!IsValid(Actor) || !Actor->Implements<USaveActor>())
					continue;
				
				FSavedActor SavedActor;
				SavedActor.ActorName = Actor->GetFName();
				SavedActor.Transform = Actor->GetTransform();
				
				FMemoryWriter MemoryWriter(SavedActor.SaveGameBytes);
				FObjectAndNameAsStringProxyArchive StringProxyArchive(MemoryWriter, true);
				StringProxyArchive.ArIsSaveGame = true;
				Actor->Serialize(StringProxyArchive);
				SavedMap.SavedActors.AddUnique(SavedActor);
			}
			FSavedMap* SavedMapPtr = SaveGameObject->SavedMaps.FindByPredicate([&SavedMap](const FSavedMap& InSavedMap) -> bool
				{
					return SavedMap.MapAssetName == InSavedMap.MapAssetName;
				});
			if (SavedMapPtr)
				*SavedMapPtr = SavedMap;
		}
		
		UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName, SlotIndex);
		return SaveGameObject;
	}

	return nullptr;
}

URPGSaveGame* ULoadManager::LoadSlotData(int32 SlotIndex)
{
	if (const FString SlotName = MakeSlotNameByIndex(SlotIndex); UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
		return Cast<URPGSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex));
	return nullptr;
}

void ULoadManager::DeleteSaveSlotData(int32 SlotIndex)
{
	if (const FString SlotName = MakeSlotNameByIndex(SlotIndex); UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
}

APlayerStart* ULoadManager::GetClosestPlayerStart() const
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if (IsValid(PlayerCharacter) && Actors.Num() > 0)
	{
		double MinDis = FVector::Distance(PlayerCharacter->GetActorLocation(), Actors[0]->GetActorLocation());
		AActor* SelectedPlayerStart = Actors[0];
		for (int32 i = 1; i < Actors.Num(); ++i)
		{
			double Dis = FVector::Distance(PlayerCharacter->GetActorLocation(), Actors[i]->GetActorLocation());
			if (MinDis >= Dis)
			{
				MinDis = Dis;
				SelectedPlayerStart = Actors[i];
			}
		}
		return Cast<APlayerStart>(SelectedPlayerStart);
	}

	return nullptr;
}

void ULoadManager::LoadActorInfoFromSaveGame(const URPGSaveGame* InSaveGame) const
{
	UWorld* CurrentWorld = GetWorld();
	if (!IsValid(CurrentWorld) || !IsValid(InSaveGame))
		return;
	
	FString WorldName = CurrentWorld->GetMapName();
	WorldName.RemoveFromStart(CurrentWorld->StreamingLevelsPrefix);
	if (!InSaveGame->HasMap(WorldName))
		return;

	const TArray<FSavedActor>& SavedActors = InSaveGame->GetSavedMapWithMapName(WorldName).SavedActors;
	for (FActorIterator It(CurrentWorld); It; ++It)
	{
		AActor* Actor = *It;
		if (!IsValid(Actor) || !Actor->Implements<USaveActor>())
			continue;

		const FSavedActor* SaveActorPtr = SavedActors.FindByPredicate([ActorName = Actor->GetFName()](const FSavedActor& InSavedActor) -> bool
			{
				return InSavedActor.ActorName == ActorName;
			});
		if (SaveActorPtr)
		{
			if (ISaveActor::Execute_ShouldLoadTransform(Actor))
				Actor->SetActorTransform(SaveActorPtr->Transform);

			FMemoryReader MemoryReader(SaveActorPtr->SaveGameBytes);
			FObjectAndNameAsStringProxyArchive StringProxyArchive(MemoryReader, true);
			StringProxyArchive.ArIsSaveGame = true;
			Actor->Serialize(StringProxyArchive);

			ISaveActor::Execute_OnActorLoaded(Actor);
		}
	}
}

FString ULoadManager::MakeSlotNameByIndex(int32 Index)
{
	return TEXT("Slot_") + FString::FromInt(Index);
}
