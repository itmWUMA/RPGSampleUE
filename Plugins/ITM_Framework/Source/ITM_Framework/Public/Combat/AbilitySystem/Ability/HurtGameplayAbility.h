// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Combat/AbilitySystem/ITMAbilityDefines.h"
#include "Combat/AbilitySystem/Ability/ITMGameplayAbility.h"
#include "HurtGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ITM_FRAMEWORK_API UHurtGameplayAbility : public UITMGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void ApplyHurtEffect(const FGameplayAbilitySpecHandle Handle, UAbilitySystemComponent* ASC, const FHurtEffectData& HurtEffectData, const FHurtDataCollection& HurtData);

	UFUNCTION()
	virtual void OnHurtAnimEnd();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GAS|Effect")
	TSubclassOf<UGameplayEffect> HurtEffectClass;

	UPROPERTY(EditAnywhere, Category = "GAS|Effect", meta = (RequiredAssetDataTags  = "RowStructure=/Script/ITM_Framework.HurtEffectDataTableRow"))
	TObjectPtr<UDataTable> HurtEffectDataTable;

private:
	FActiveGameplayEffectHandle HurtEffectHandle;
	FTimerHandle HurtEffectTimerHandle;
};
