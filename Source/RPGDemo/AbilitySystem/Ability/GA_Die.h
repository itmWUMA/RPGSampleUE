// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Combat/AbilitySystem/Ability/ITMGameplayAbility.h"
#include "GA_Die.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "Die Gameplay Ability")
class RPGDEMO_API UGA_Die : public UITMGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	void OnStartDestroyCharacter();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	float DestroyCharacterDelayTime = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "Death")
	FGameplayTag DeathCueTag = FGameplayTag::EmptyTag;

private:
	FTimerHandle DestroyDelayHandle;
};
