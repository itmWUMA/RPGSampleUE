// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfoDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 LevelUpRequirement = 0;

	UPROPERTY(EditAnywhere)
	int32 AttributePointReward = 1;
};

/**
 * 
 */
UCLASS(BlueprintType)
class RPGDEMO_API ULevelUpInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "AbilitySystem|XP")
	int32 GetLevelForXP(int32 InXP) const;

	UFUNCTION(BlueprintPure, Category = "AbilitySystem|XP")
	int32 GetAttributePointReward(int32 InLevel) const;

public:
	UPROPERTY(EditAnywhere, meta = (ToolTip = "数组中等级消耗值应当从小到大有序"))
	TArray<FLevelUpInfo> LevelUpInfos;
};
