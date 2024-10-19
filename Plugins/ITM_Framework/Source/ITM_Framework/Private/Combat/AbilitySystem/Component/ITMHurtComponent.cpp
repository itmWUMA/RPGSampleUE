// Copyright 2024 Itmwuma


#include "Combat/AbilitySystem/Component/ITMHurtComponent.h"

#include "AbilitySystemComponent.h"
#include "Settings/ITMFrameworkSettings.h"

void UITMHurtComponent::BeginPlay()
{
	Super::BeginPlay();

	ASCCache = GetOwner()->GetComponentByClass<UAbilitySystemComponent>();

	if (ASCCache.IsValid())
	{
		if (const UITMFrameworkSettings* Settings = UITMFrameworkSettings::Get())
		{
			OnHurtTagChangedHandle =
				ASCCache->RegisterGameplayTagEvent(Settings->HurtTag).AddUObject(this, &UITMHurtComponent::OnHurtTagChanged);
		}
	}
}

void UITMHurtComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ASCCache.IsValid())
	{
		if (const UITMFrameworkSettings* Settings = UITMFrameworkSettings::Get())
		{
			ASCCache->RegisterGameplayTagEvent(Settings->HurtTag).Remove(OnHurtTagChangedHandle);
		}
	}
	
	Super::EndPlay(EndPlayReason);
}

void UITMHurtComponent::OnHurtTagChanged(const FGameplayTag HurtTag, int32 NewCount)
{
	bHurting = NewCount > 0;

	if (!bHurting)
	{
		CurrentHurtData = FHurtDataCollection();
	}

	OnHurtDelegate.Broadcast(bHurting);
}

