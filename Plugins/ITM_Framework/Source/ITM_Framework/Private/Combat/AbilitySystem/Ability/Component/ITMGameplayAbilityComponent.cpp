// Copyright 2024 Itmwuma


#include "Combat/AbilitySystem/Ability/Component/ITMGameplayAbilityComponent.h"
#include "Combat/AbilitySystem/Ability/ITMGameplayAbility.h"

void UITMGameplayAbilityComponent::InitComponent(UITMGameplayAbility* InOwnerAbility)
{
	OwnerAbility = InOwnerAbility;
}
