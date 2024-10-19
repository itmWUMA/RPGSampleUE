// Copyright 2024 Itmwuma

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayAbilitySpec.h"
#include "GameplayAbilitySpecHandle.h"
#include "InputActionValue.h"
#include "ITMInputConfig.h"
#include "Components/ActorComponent.h"
#include "AbilityBindingComponent.generated.h"

USTRUCT()
struct FAbilityBindingDataWrapper
{
	GENERATED_BODY()
	
	UPROPERTY(Transient)
	UAbilityInputBindingObject* BindingObject = nullptr;

	FGameplayAbilitySpecHandle SpecHandle;
};

class UAbilitySystemComponent;
class UITMInputConfig;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ITM_FRAMEWORK_API UAbilityBindingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAbilityBindingComponent();

	void BindAbilityInput(UEnhancedInputComponent* InputComponent, FGameplayAbilitySpec& Spec);
	void BindAbilityInput(UEnhancedInputComponent* InputComponent, const FGameplayTag& InputTag, FGameplayAbilitySpec& Spec);
	void BindDefaultAbilities(UEnhancedInputComponent* InputComponent);

	bool GetBindingDataWrapperByTag(const FGameplayTag& InputTag, FAbilityBindingDataWrapper& OutPolicyData);
	
	static FGameplayTag GetDefaultInputTag(UAbilitySystemComponent* InASC, const FGameplayAbilitySpec& InSpec);

protected:
	virtual void BeginPlay() override;

private:
	void AbilityInputBinding_Triggered(const FInputActionValue& Value, const FGameplayTag InInputTag);
	void AbilityInputBinding_Started(const FInputActionValue& Value, const FGameplayTag InInputTag);
	void AbilityInputBinding_Ongoing(const FInputActionValue& Value, const FGameplayTag InInputTag);
	void AbilityInputBinding_Completed(const FInputActionValue& Value, const FGameplayTag InInputTag);
	void AbilityInputBinding_Canceled(const FInputActionValue& Value, const FGameplayTag InInputTag);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UITMInputConfig> InputConfig;

private:
	UPROPERTY(Transient)
	TMap<FGameplayTag, FAbilityBindingDataWrapper> BindingDataMap;
	
	TWeakObjectPtr<UAbilitySystemComponent> ASCCache = nullptr;
};
