// Copyright 2024 Itmwuma


#include "Input/AbilityInputBindingObject.h"

#include "AbilitySystemComponent.h"
#include "Combat/AbilitySystem/Ability/ITMGameplayAbility.h"

void UAbilityInputBindingObject::OnTriggered_Implementation(const FInputActionValue& InputActionValue,
                                                       const FGameplayTag& InputTag, const FGameplayAbilitySpecHandle& SpecHandle)
{
	ActivateAbility(ETriggerEvent::Triggered, SpecHandle);
	EndAbilityNotify(ETriggerEvent::Triggered);
	CurrentInputVector = InputActionValue.Get<FVector>();
}

void UAbilityInputBindingObject::OnStarted_Implementation(const FInputActionValue& InputActionValue,
	const FGameplayTag& InputTag, const FGameplayAbilitySpecHandle& SpecHandle)
{
	ActivateAbility(ETriggerEvent::Started, SpecHandle);
	EndAbilityNotify(ETriggerEvent::Started);
	CurrentInputVector = InputActionValue.Get<FVector>();
}

void UAbilityInputBindingObject::OnGoing_Implementation(const FInputActionValue& InputActionValue,
	const FGameplayTag& InputTag, const FGameplayAbilitySpecHandle& SpecHandle)
{
	ActivateAbility(ETriggerEvent::Ongoing, SpecHandle);
	EndAbilityNotify(ETriggerEvent::Ongoing);
}

void UAbilityInputBindingObject::OnCompleted_Implementation(const FInputActionValue& InputActionValue,
	const FGameplayTag& InputTag, const FGameplayAbilitySpecHandle& SpecHandle)
{
	ActivateAbility(ETriggerEvent::Completed, SpecHandle);
	EndAbilityNotify(ETriggerEvent::Completed);
	CurrentInputVector = FVector::ZeroVector;
}

void UAbilityInputBindingObject::OnCanceled_Implementation(const FInputActionValue& InputActionValue,
	const FGameplayTag& InputTag, const FGameplayAbilitySpecHandle& SpecHandle)
{
	ActivateAbility(ETriggerEvent::Canceled, SpecHandle);
	EndAbilityNotify(ETriggerEvent::Canceled);
	CurrentInputVector = FVector::ZeroVector;
}

void UAbilityInputBindingObject::ActivateAbility(ETriggerEvent InActivateAbilityOpportunity, const FGameplayAbilitySpecHandle& SpecHandle)
{
	if (InActivateAbilityOpportunity != ActivateAbilityOpportunity ||
		!ASCCache.IsValid() ||
		(!bCanReactive && bActivated))
	{
		return;
	}

	if (ASCCache->TryActivateAbility(SpecHandle))
	{
		if (FGameplayAbilitySpec* Spec = ASCCache->FindAbilitySpecFromHandle(SpecHandle))
		{
			if (UITMGameplayAbility* GAInstance = Cast<UITMGameplayAbility>(Spec->GetPrimaryInstance()))
			{
				GAInstance->RegisterAbilityBindingPolicy();
			}
		}
		
		OnActivateAbility();
	}
}

void UAbilityInputBindingObject::OnActivateAbility()
{
	bActivated = true;
}

void UAbilityInputBindingObject::EndAbilityNotify(ETriggerEvent InEndAbilityOpportunity)
{
	if (!bEndAbilityByPolicy ||
		InEndAbilityOpportunity != EndAbilityOpportunity ||
		!bActivated)
	{
		return;
	}

	if (OnBindingObjectEndAbilityNotifyDelegate.ExecuteIfBound())
		OnEndAbilityNotify();
}

void UAbilityInputBindingObject::OnEndAbilityNotify()
{
	bActivated = false;
}

UAbilityInputBindingObject_Pressed::UAbilityInputBindingObject_Pressed()
{
	ActivateAbilityOpportunity = ETriggerEvent::Triggered;
	bEndAbilityByPolicy = false;
}

UAbilityInputBindingObject_Holding::UAbilityInputBindingObject_Holding()
{
	ActivateAbilityOpportunity = ETriggerEvent::Triggered;
	bEndAbilityByPolicy = true;
	EndAbilityOpportunity = ETriggerEvent::Completed;
}
