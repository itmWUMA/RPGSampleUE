// Copyright 2024 Itmwuma


#include "Combat/AbilitySystem/Task/WaitAbilityCooldownChanged.h"

#include "AbilitySystemComponent.h"

UWaitAbilityCooldownChanged* UWaitAbilityCooldownChanged::WaitAbilityCooldownChanged(UAbilitySystemComponent* ASC,
                                                                                     const FGameplayTag& CooldownTag)
{
	UWaitAbilityCooldownChanged* Proxy = NewObject<UWaitAbilityCooldownChanged>();
	Proxy->ASCCache = ASC;
	Proxy->CooldownTagCache = CooldownTag;

	if (!IsValid(ASC))
	{
		Proxy->EndTask();
		return nullptr;
	}

	ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(Proxy,
		&UWaitAbilityCooldownChanged::OnCooldownTagChanged);

	return Proxy;
}

void UWaitAbilityCooldownChanged::EndTask()
{
	if (ASCCache)
	{
		ASCCache->RegisterGameplayTagEvent(CooldownTagCache).RemoveAll(this);
		SetReadyToDestroy();
		MarkAsGarbage();
	}
}

void UWaitAbilityCooldownChanged::OnCooldownTagChanged(UAbilitySystemComponent* ASC,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle)
{
	FGameplayTagContainer GrantTags;
	EffectSpec.GetAllGrantedTags(GrantTags);
	if (GrantTags.HasTagExact(CooldownTagCache))
	{
		CooldownStart.Broadcast(EffectSpec.GetDuration());
	}
}
