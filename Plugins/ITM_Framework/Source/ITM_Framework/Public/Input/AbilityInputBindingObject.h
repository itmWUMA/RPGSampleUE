// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "UObject/NoExportTypes.h"
#include "EnhancedInput/Public/InputTriggers.h"
#include "AbilityInputBindingObject.generated.h"

class UAbilityBindingComponent;
class UAbilitySystemComponent;

DECLARE_DELEGATE(FOnBindingObjectEndAbilityNotify);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class ITM_FRAMEWORK_API UAbilityInputBindingObject : public UObject
{
	GENERATED_BODY()

	friend class UAbilityBindingComponent;
	friend class UITMGameplayAbility;

public:
	UAbilityInputBindingObject() = default;
	UAbilityInputBindingObject(TWeakObjectPtr<UAbilitySystemComponent> InASCCache) : ASCCache(InASCCache) { }
	
	UFUNCTION(BlueprintNativeEvent)
	void OnTriggered(const FInputActionValue& InputActionValue, const FGameplayTag& InputTag, const FGameplayAbilitySpecHandle& SpecHandle);
	virtual void OnTriggered_Implementation(const FInputActionValue& InputActionValue, const FGameplayTag& InputTag, const FGameplayAbilitySpecHandle& SpecHandle);

	UFUNCTION(BlueprintNativeEvent)
	void OnStarted(const FInputActionValue& InputActionValue, const FGameplayTag& InputTag, const FGameplayAbilitySpecHandle& SpecHandle);
	virtual void OnStarted_Implementation(const FInputActionValue& InputActionValue, const FGameplayTag& InputTag, const FGameplayAbilitySpecHandle& SpecHandle);

	UFUNCTION(BlueprintNativeEvent)
	void OnGoing(const FInputActionValue& InputActionValue, const FGameplayTag& InputTag, const FGameplayAbilitySpecHandle& SpecHandle);
	virtual void OnGoing_Implementation(const FInputActionValue& InputActionValue, const FGameplayTag& InputTag, const FGameplayAbilitySpecHandle& SpecHandle);

	UFUNCTION(BlueprintNativeEvent)
	void OnCompleted(const FInputActionValue& InputActionValue, const FGameplayTag& InputTag, const FGameplayAbilitySpecHandle& SpecHandle);
	virtual void OnCompleted_Implementation(const FInputActionValue& InputActionValue, const FGameplayTag& InputTag, const FGameplayAbilitySpecHandle& SpecHandle);

	UFUNCTION(BlueprintNativeEvent)
	void OnCanceled(const FInputActionValue& InputActionValue, const FGameplayTag& InputTag, const FGameplayAbilitySpecHandle& SpecHandle);
	virtual void OnCanceled_Implementation(const FInputActionValue& InputActionValue, const FGameplayTag& InputTag, const FGameplayAbilitySpecHandle& SpecHandle);

	UFUNCTION(BlueprintPure, Category = "GAS|Input")
	UAbilitySystemComponent* GetAbilitySystemComponent() const { return ASCCache.Get(); }

	UFUNCTION(BlueprintPure, Category = "GAS|Input")
	FVector GetInputVector() const { return CurrentInputVector; }

	FORCEINLINE FOnBindingObjectEndAbilityNotify& GetOnBindingPolicyEndAbilityNotifyDelegate() { return OnBindingObjectEndAbilityNotifyDelegate; }

protected:
	virtual void ActivateAbility(ETriggerEvent InActivateAbilityOpportunity, const FGameplayAbilitySpecHandle& SpecHandle);
	virtual void OnActivateAbility();
	virtual void EndAbilityNotify(ETriggerEvent InEndAbilityOpportunity);
	virtual void OnEndAbilityNotify();

protected:
	UPROPERTY(EditDefaultsOnly)
	ETriggerEvent ActivateAbilityOpportunity = ETriggerEvent::Triggered;

	UPROPERTY(EditDefaultsOnly, meta = (InlineEditConditionToggle))
	bool bEndAbilityByPolicy = false;
	
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bEndAbilityByPolicy"))
	ETriggerEvent EndAbilityOpportunity = ETriggerEvent::Completed;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "是否支持在当前能力已激活后再次通过输入激活能力"))
	bool bCanReactive = false;

private:
	TWeakObjectPtr<UAbilitySystemComponent> ASCCache = nullptr;
	FVector CurrentInputVector = FVector::ZeroVector;
	bool bActivated = false;
	FOnBindingObjectEndAbilityNotify OnBindingObjectEndAbilityNotifyDelegate;
};

UCLASS(DisplayName = "Ability Type (Pressed)")
class ITM_FRAMEWORK_API UAbilityInputBindingObject_Pressed : public UAbilityInputBindingObject
{
	GENERATED_BODY()

public:
	UAbilityInputBindingObject_Pressed();
};

UCLASS(DisplayName = "Ability Type (Holding)")
class ITM_FRAMEWORK_API UAbilityInputBindingObject_Holding : public UAbilityInputBindingObject
{
	GENERATED_BODY()

public:
	UAbilityInputBindingObject_Holding();
};