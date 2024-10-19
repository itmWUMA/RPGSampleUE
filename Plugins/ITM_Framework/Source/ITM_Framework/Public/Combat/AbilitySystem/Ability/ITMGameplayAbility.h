// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ITMGameplayAbility.generated.h"

class UITMGameplayAbilityComponent;
class UAbilityInputBindingObject;

/**
 * 
 */
UCLASS()
class ITM_FRAMEWORK_API UITMGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UITMGameplayAbility();

	FORCEINLINE FGameplayTag GetDefaultInputTag() const { return DefaultInputTag; }
	virtual void RegisterAbilityBindingPolicy();

	UFUNCTION(BlueprintPure, Category = "GAS|Input")
	UAbilityInputBindingObject* GetInputBindingObject() const;

	UFUNCTION(BlueprintPure, Category = "GAS|Input")
	FGameplayTag GetCurrentInputTag() const;

	virtual void TryEndAbility(bool bWasCancelled = false);

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	void ListenEndAbilityNotifyFromInput();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (ToolTip="默认输入Tag，并非实际的InputTag，运行时可动态调整"))
	FGameplayTag DefaultInputTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Component")
	TArray<TObjectPtr<UITMGameplayAbilityComponent>> GameplayAbilityComponents;

private:
	TWeakObjectPtr<UAbilityInputBindingObject> BindingObjectCache = nullptr;
};
