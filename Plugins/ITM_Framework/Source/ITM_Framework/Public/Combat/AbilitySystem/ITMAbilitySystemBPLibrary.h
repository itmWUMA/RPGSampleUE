// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ITMAbilitySystemBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ITM_FRAMEWORK_API UITMAbilitySystemBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 获取当前能力的InputTag
	UFUNCTION(BlueprintPure, Category = "GAS|Input")
	static FGameplayTag GetCurrentInputTag(const FGameplayAbilitySpec& Spec);
};
