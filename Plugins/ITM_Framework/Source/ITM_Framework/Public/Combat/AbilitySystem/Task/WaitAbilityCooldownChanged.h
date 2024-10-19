// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitAbilityCooldownChanged.generated.h"

class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityCooldownChanged, float, RemainTime);

/**
 * 
 */
UCLASS()
class ITM_FRAMEWORK_API UWaitAbilityCooldownChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitAbilityCooldownChanged* WaitAbilityCooldownChanged(UAbilitySystemComponent* ASC, const FGameplayTag& CooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	void OnCooldownTagChanged(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle);
	
protected:
	UPROPERTY(BlueprintAssignable)
	FOnAbilityCooldownChanged CooldownStart;

	UPROPERTY(Transient)
	TObjectPtr<UAbilitySystemComponent> ASCCache = nullptr;

	FGameplayTag CooldownTagCache = FGameplayTag::EmptyTag;
};
