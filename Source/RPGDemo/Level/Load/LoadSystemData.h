// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "RPGSaveGame.h"
#include "Engine/DataAsset.h"
#include "UI/ITMUserInterfaceDefines.h"
#include "LoadSystemData.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API ULoadSystemData : public UDataAsset
{
	GENERATED_BODY()

public:
	TSoftObjectPtr<UWorld> GetMapByName(const FString& InString) const;

public:
	UPROPERTY(EditAnywhere)
	FITMWidgetClassConfig LoadWidgetData;

	UPROPERTY(EditAnywhere)
	FITMWidgetClassConfig SaveWidgetData;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<URPGSaveGame> SaveGameClass;

	UPROPERTY(EditAnywhere)
	FString NewGameLevel = TEXT("");

	UPROPERTY(EditAnywhere)
	FName DefaultPlayerStartTag = NAME_None;

	UPROPERTY(EditAnywhere)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;
};
