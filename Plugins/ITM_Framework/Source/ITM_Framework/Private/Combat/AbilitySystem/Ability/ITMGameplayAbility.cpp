// Copyright 2024 Itmwuma


#include "Combat/AbilitySystem/Ability/ITMGameplayAbility.h"

#include "Combat/AbilitySystem/ITMAbilitySystemUtilities.h"
#include "Combat/AbilitySystem/Ability/Component/ITMGameplayAbilityComponent.h"
#include "Core/ITMPlayerController.h"
#include "Input/AbilityBindingComponent.h"
#include "Settings/ITMFrameworkSettings.h"

UITMGameplayAbility::UITMGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UITMGameplayAbility::RegisterAbilityBindingPolicy()
{
	if (BindingObjectCache.IsValid())
		return;

	if (UAbilityInputBindingObject* BindingObject = GetInputBindingObject())
	{
		BindingObjectCache = BindingObject;
		ListenEndAbilityNotifyFromInput();
	}
}

UAbilityInputBindingObject* UITMGameplayAbility::GetInputBindingObject() const
{
	if (AITMPlayerController* PC = GetAvatarActorFromActorInfo()->GetInstigatorController<AITMPlayerController>())
	{
		if (UAbilityBindingComponent* AbilityBindingComp = PC->GetAbilityBindingComp())
		{
			FAbilityBindingDataWrapper Data;
			FGameplayTag InputTag = GetCurrentInputTag();
			if (AbilityBindingComp->GetBindingDataWrapperByTag(InputTag, Data))
			{
				return Data.BindingObject;
			}
		}
	}
	return nullptr;
}

FGameplayTag UITMGameplayAbility::GetCurrentInputTag() const
{
	if (const FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec())
	{
		return FITMAbilitySystemUtilities::GetCurrentInputTag(*Spec);
	}
	
	return FGameplayTag::EmptyTag;
}

void UITMGameplayAbility::TryEndAbility(bool bWasCancelled)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, bWasCancelled);
}

void UITMGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	for (UITMGameplayAbilityComponent* AbilityComp : GameplayAbilityComponents)
	{
		AbilityComp->InitComponent(this);
		AbilityComp->OnBegin(Handle, *ActorInfo);
	}
}

void UITMGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     bool bReplicateEndAbility, bool bWasCancelled)
{
	for (UITMGameplayAbilityComponent* AbilityComp : GameplayAbilityComponents)
	{
		AbilityComp->OnEnd(Handle, *ActorInfo);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (BindingObjectCache.IsValid())
	{
		BindingObjectCache->bActivated = false;
	}
}

void UITMGameplayAbility::ListenEndAbilityNotifyFromInput()
{
	if (!BindingObjectCache.IsValid() ||
		!BindingObjectCache->bEndAbilityByPolicy)
	{
		return;
	}

	BindingObjectCache->GetOnBindingPolicyEndAbilityNotifyDelegate().BindLambda([WeakThis = TWeakObjectPtr<UITMGameplayAbility>(this)]()
	{
		WeakThis->K2_EndAbility();
	});
}
