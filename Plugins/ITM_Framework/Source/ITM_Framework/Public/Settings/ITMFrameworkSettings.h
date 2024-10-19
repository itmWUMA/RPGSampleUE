// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "ITMFrameworkSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, DisplayName = "ITM Framework Settings")
class ITM_FRAMEWORK_API UITMFrameworkSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetContainerName() const override { return "Project"; }
	virtual FName GetCategoryName() const override { return "Plugins"; }
	virtual FName GetSectionName() const override { return "ITM Framework Settings"; }

	static const UITMFrameworkSettings* Get();

public:
	UPROPERTY(Config, EditAnywhere, Category = "GAS|Input")
	FGameplayTag DefaultInputBaseTag = FGameplayTag::EmptyTag;

	UPROPERTY(Config, EditAnywhere, Category = "Projectile")
	FName LaunchObjectOverlapTag = TEXT("LaunchObjectOverlap");

	UPROPERTY(Config, EditAnywhere, Category = "GAS|Damage")
	FGameplayTag DamageTag = FGameplayTag::EmptyTag;

	UPROPERTY(Config, EditAnywhere, Category = "GAS|Damage")
	FGameplayTag HurtTag = FGameplayTag::EmptyTag;
};
