// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RPGSaveGame.generated.h"

UENUM(BlueprintType)
enum class ESaveSlotStatus : uint8
{
	Empty,
	Taken,
};

USTRUCT()
struct FSavedActor
{
	GENERATED_BODY()

	UPROPERTY()
	FName ActorName = NAME_None;

	UPROPERTY()
	FTransform Transform = FTransform::Identity;

	UPROPERTY()
	TArray<uint8> SaveGameBytes;

	bool operator==(const FSavedActor& InSavedActor) const;
};

USTRUCT()
struct FSavedMap
{
	GENERATED_BODY()

	UPROPERTY()
	FString MapAssetName = TEXT("");

	UPROPERTY()
	TArray<FSavedActor> SavedActors;
};

/**
 * 
 */
UCLASS()
class RPGDEMO_API URPGSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	FSavedMap GetSavedMapWithMapName(const FString& InMapName) const;
	bool HasMap(const FString& InMapName) const;

public:
	UPROPERTY()
	FString SlotName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	FString PlayerName = FString("Default Name");

	UPROPERTY()
	FDateTime SlotDateTime;

	UPROPERTY()
	FString MapName = FString();

	UPROPERTY()
	FName PlayerStartTag = NAME_None;

	UPROPERTY()
	int32 PlayerLevel = 0;

	UPROPERTY()
	int32 XP = 0;

	UPROPERTY()
	int32 AttributePoint = 0;

	UPROPERTY()
	float Strength = 0;

	UPROPERTY()
	float Intelligence = 0;

	UPROPERTY()
	float Resilience = 0;

	UPROPERTY()
	float Vigor = 0;

	UPROPERTY()
	TArray<FSavedMap> SavedMaps;
};
