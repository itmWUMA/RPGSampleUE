// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Combat/AbilitySystem/Ability/AttackGameplayAbility.h"
#include "GA_MeleeAttack.generated.h"

class UAbilityTask_WaitGameplayEvent;
/**
 * 
 */
UCLASS()
class RPGDEMO_API UGA_MeleeAttack : public UAttackGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	virtual void OnHitDetectTagReceived(FGameplayEventData Payload);
	
	UFUNCTION(BlueprintPure, Category = "Combat|Attack")
	bool CanAttack(const AActor* Instigator, const AActor* Target) const;

	UFUNCTION(BlueprintCallable, Category = "Combat|Attack")
	void AttackTarget(const AActor* Instigator, AActor* Target);

protected:
	UPROPERTY(EditAnywhere, Category = "Combat|Attack")
	FGameplayTag HitDetectTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditAnywhere, Category = "Combat|Attack")
	bool bOnlyAttackCombatTarget = true;

	UPROPERTY(EditAnywhere, Category = "Combat|Attack")
	FGameplayTag ImpactCueTag = FGameplayTag::EmptyTag;

	UPROPERTY(Transient)
	UAbilityTask_WaitGameplayEvent* WaitGameplayEventTask = nullptr;
};
